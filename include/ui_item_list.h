#ifndef UI_ITEM_LIST_H
#define UI_ITEM_LIST_H

#include "connectorlist.h"
#include "fxsparamlist.h"
#include "textstuff.h"
#include "types.h"
#include "ui_item.h"

#include <string>

// currently ui_item.h contains all the information about how this works.


class connector;

namespace wcnt {
    fxsparamlist*   get_fxsparamlist();
};

namespace ui
{
 #ifdef DEBUG
 #define ui_err(fmt, ... )                              \
 {                                                       \
    cfmt(item_list<T>::err_msg, STRBUFLEN, "%40s:%5d %-35s\n" fmt, \
                    __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);   \
 }
 #else
 #define ui_err(fmt, ... ) \
    cfmt(item_list<T>::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
 #endif

 // T will be a pointer to synthmod::base or dobj::base


 template <class T>
 class item_list : public linked_list< base<T> >
 {
  public:
    item_list();
    ~item_list() {};

    typedef ll_item< base<T> > llitem;
    typedef linked_list< base<T> > linkedlist;

    base<T>* add_item(input::TYPE);
    base<T>* add_item(param::TYPE);
    base<T>* add_item(param::TYPE, const char* fixed_str);
    base<T>* add_item(dobj::TYPE parent, dobj::TYPE child);
    base<T>* add_item(const char* comment);

    // specialist method, used by synthmod::base, dobj::base impl is empty.
    connector* add_connector_off(T, input::TYPE);


    bool     quick_scan(const char*);
    // call match_begin at the start of each new module/data object to
    // be read.
    void     match_begin(T);
    base<T>* match_item(const char*);

    // confirm all non-optional items have been matched etc,
    // returns 0 on success or error<T>::err();
    base<T>* validate_matches();

    // validates data for parameters, (T is the container)
    bool data_validate(T, param::TYPE, errors::TYPE);

  private:
    T       subject;

    FLAGS    skip_id;
    FLAGS    match_id;
    base<T>* item;
    base<T>* prev;
    base<T>* last;

    llitem*  group0;
    FLAGS    group0id;
    base<T>* grouplast;

    struct multiplechoice
    {
        llitem* head;       // first item of first option
        llitem* opt0;       // first item of current option
        llitem* chosen0;    // first item of chosen option
        base<T>* last;      // gone full circle
        base<T>* tail;      // last item of last option
        FLAGS opt0id;
        FLAGS chosen0id;

        void reset() {
            head = opt0 = chosen0 = 0;
            last = tail = 0;
            opt0id = chosen0id = UI_DEFAULT;
        }

        #ifdef DEBUG
        void dump() {
            std::cout << "option:\thead " << (void*) head;
            if (head)
                std::cout << " ('" << head->get_data()->get_name() << "')";
            std::cout << "\topt0 " << (void*) opt0;
            if (opt0)
                std::cout << " ('" << opt0->get_data()->get_name() << "')";
            std::cout << "\tchosen0 " << (void*) chosen0;
            if (chosen0)
                std::cout << " ('" << chosen0->get_data()->get_name() << "')";
            std::cout << std::endl;
            std::cout << "\tlast " << (void*) last;
            if (last)
                std::cout << " ('" << last->get_name() << "')";
            std::cout << "\ttail " << (void*) tail;
            if (tail)
                std::cout << " ('" << tail->get_name() << "')";
            std::cout << std::endl;
        }
        #endif

    } choice;


    base<T>* match_item_choice(const char*);
    bool     match_item_choice_is_valid();
    base<T>* match_item_chosen(const char*);
    bool     match_item_choice_all_present(const char*);
    void     match_item_chosen_invalid_error();

    void goto_choice_head() {
        item = this->goto_item(choice.head);
        choice.opt0 = choice.head;
        choice.opt0id = item->get_option_id();
    }

    void skip_choice() {
        FLAGS cid = item->get_option_id();
        do {
            item = this->goto_next();
            if (!item) {
                item = this->goto_first();
                break;
            }
            else if (this->sneak_current() == choice.head)
                break;
            else if (item->get_item_type() != UI_COMMENT) {
                FLAGS id = item->get_option_id();
                if ((id == UI_OPTION1 && cid > UI_OPTION1)
                 || id == UI_DEFAULT)
                    break;
                cid = id;
            }
        } while(true);
        choice.reset();
    }

    bool validate_matches_choice();

    enum { LIST_STD = 0xff, LIST_EDIT = 0xfff };
    bool goodlist(int t) {
        if (listtype == 0)
            listtype = t;
        return (listtype == t);
    }

    int listtype;
    static char err_msg[STRBUFLEN];
 };


 template <class T>
 item_list<T>::item_list() :
 linkedlist(MULTIREF_OFF),
 subject(0),
 skip_id(UI_DEFAULT), match_id(UI_DEFAULT),
 item(0), prev(0), last(0),
  listtype(0)
 {
    std::cout << "item_list<T>::item_list<T>()" << std::endl;
    std::cout << " + err_msg: " << (void*)err_msg << std::endl;
    error_item<T>::err(err_msg);
 }

 template <class T>
 base<T>* item_list<T>::add_item(param::TYPE pt)
 {
    if (pt == param::STR_UNNAMED || pt == param::STR_LIST) {
        if (!goodlist(LIST_EDIT)) {
            std::cout << "WE're *UNHAPPY AND WE KNOW IT!" << std::endl;
            return 0;
        }
        else
            std::cout << "WE ARE VERY HAPPY AND WE KNOW IT!" << std::endl;
    }
    else if (!goodlist(LIST_STD))
        return 0;

    base<T>* i = new param_item<T>(pt);
    if (i) {
        if (!this->add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }

    i->dump();

    return i;
 }

 template <class T>
 base<T>* item_list<T>::add_item(param::TYPE pt, const char* fixstr)
 {
    if (!goodlist(LIST_STD))
        return 0;

    if (!fixstr)
        return 0;

    base<T>* i = new param_item<T>(pt);
    if (i) {
        llitem * li = this->add_at_tail(i);
        if (li) {
            if (!wcnt::get_fxsparamlist()->add_param(fixstr, pt)) {
                delete this->unlink_item(li);
                delete i;
                i = 0;
            }
        }
        else {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 template <class T>
 base<T>* item_list<T>::add_item(dobj::TYPE parent, dobj::TYPE child)
 {
    if (!goodlist(LIST_STD))
        return 0;
    base<T>* i = new dobj_item<T>(parent, child);
    if (i) {
        if (!this->add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 template <class T>
 base<T>* item_list<T>::add_item(const char* comment)
 {
    base<T>* i = new comment_item<T>(comment);
    if (i) {
        if (!this->add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 template <class T>
 bool item_list<T>::quick_scan(const char* str)
 {
    std::cout << "QUICK SCAN:'" << str << "'...";
    llitem* li = this->sneak_first();
    do {
        base<T>* it = li->get_data();
        if (it->is_name_match(str)) {
            std::cout << "OK." << std::endl;
            return true;
        }
        li = li->get_next();
    } while (li);
    std::cout << "FAIL." << std::endl;
    return false;
 }

 template <class T>
 void item_list<T>::match_begin(T t)
 {
    std::cout << "\n--------------------------------------------------\n"
              << "match_begin()" << std::endl;

    subject = t;

    if (!(item = this->goto_first()))
        return;

    #ifdef DEBUG
    int n = 0;
    #endif

    do {
        item->reset_matched();
        #ifdef DEBUG
        llitem* li = this->sneak_current();
        std::cout << "ll_item: " << (void*)li << "\titem: " << ++n << " - ";
        item->dump();
        #endif
    } while ((item = this->goto_next()) != 0);

    choice.reset();

    group0id = UI_DEFAULT;
    prev = last = 0;
    group0 = 0;
    item = this->goto_first();
 }


 template <class T>
 base<T>* item_list<T>::match_item(const char* str)
 {
    if (!listtype || !str || this->is_empty())
        return 0;

    std::cout << "match_item(" << str << ")" << std::endl;

    base<T>* failchoice = 0;

restart:
    if (!item)
        item = this->goto_first();
    std::cout << "passed restart" << std::endl;
    std::cout << "item dump: "; item->dump();
    std::cout << "last dump: ";
    if (last)
        last->dump();
    else
        std::cout << "NULL" << std::endl;

    if (last == item) {
        if (!(item = this->goto_next()))
            item = this->goto_first();
    }

    if (listtype == LIST_EDIT) {
        last = item;
        return item;
    }

    if (choice.chosen0) {
        std::cout << "\t(CHOICE.CHOSEN0)" << std::endl;
        base<T>* ic = match_item_chosen(str);
        if (ic)
            return ic;
        failchoice = choice.head->get_data();
        skip_choice();
    }
    if (choice.opt0) {
        std::cout << "\t(CHOICE.CUR0)" << std::endl;
        base<T>* ic = match_item_choice(str);
        if (ic)
            return ic;
        failchoice = choice.head->get_data();
        skip_choice();
    }
/*    else if (group0)
        return match_item_group(str, item);
*/
    do {
        if (item->get_item_type() != UI_COMMENT) {

            #ifdef DEBUG
            std::cout << "match_item loop on: ";
            item->dump();
            #endif

            FLAGS id = item->get_option_id();

            if (id & UI_OPTION_MASK) {
                if (item == failchoice) {
                    ui_err("Invalid item '%s' not found.", str);
                    return error_item<T>::err();
                }
                choice.chosen0 = 0;
                choice.last = choice.tail = 0;
                choice.head = choice.opt0 = this->sneak_current();
                choice.opt0id = id;
                choice.chosen0id = UI_DEFAULT;
                std::cout << "/////////// choice.head llitem: " << (void*) choice.head << std::endl;
                base<T>* ic = match_item_choice(str);
                if (ic)
                    return ic;
                std::cout << "resetting choice before restart..." << std::endl;
                choice.reset();
                goto restart;
            }
/*
            id = item->get_group_id();

            if (id & UI_GROUP_MASK) {
                group0 = this->sneak_current();
                group0id = id;
                grouplast = 0;
                return match_item_group(str, item);
            }
*/
            if (item->is_name_match(str)) {
                if (item->is_matched()) {
                    ui_err("Duplicate item: %s.", item->get_name());
                    return error_item<T>::err();
                }
                std::cout << "item matched, not choice/group" << std::endl;
                item->set_matched();
                last = item;
                return item;
            }
        }
        else {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
        }

        item = this->goto_next();
        if (last && !item)
            item = this->goto_first();

    } while (item != last);

    ui_err("No item found matching '%s'.", str);
    return error_item<T>::err();
 }


// match_item_choice, called by match_item, when choice0 is non-zero
// choice0 is only non-zero once a string has been matched with a choice-item
//
 template <class T>
 base<T>* item_list<T>::match_item_choice(const char* str)
 {
    std::cout << "match_item_choice(str:\"" << str << "\", item:'" << item->get_name() << "')" << std::endl;
    std::cout << "-------------------- choice.head llitem: " << (void*) choice.head << std::endl;
    if (choice.last == item) {
        if (!(item = this->goto_next()))
            this->goto_choice_head();
    }

    do {
        if (item->get_item_type() != UI_COMMENT) {

            #ifdef DEBUG
            std::cout << "   match_item_choice ";
            item->dump();
            #endif

            FLAGS id = item->get_option_id();
            if (id & UI_OPTION_MASK) {
                if (!choice.tail) {
                    if (choice.opt0id > UI_OPTION1 && id == UI_OPTION1) {
                        std::cout << "set choice.tail (via new choice selection).." << std::endl;
                        choice.tail = item;
                        this->goto_choice_head();
                        continue;
                    }
                }
                else if (item == choice.tail) {
                    if (item) {
                        std::cout << "item == choice.tail"; item->dump();
                    }
                    this->goto_choice_head();
                    continue;
                }
                if (id > choice.opt0id) {
                    choice.opt0 = this->sneak_current();
                    choice.opt0id = id;
                }

                if (item->is_name_match(str)) {
                    if (item->is_matched()) {
                        ui_err("Item '%s' already specified.", item->get_name());
                        return error_item<T>::err();
                    }
                    if (!match_item_choice_is_valid()) {
                        std::cout << "match item choice is invalid" << std::endl;
                        ui_err("Invalid item after previously %s",
                                        "selecting choice item");
                        return error_item<T>::err();
                    }
                    item->set_matched();
                    choice.chosen0 = choice.opt0;
                    choice.chosen0id = choice.opt0id;
                    choice.last = item;
                    return item;
                }
            }
            else {
                if (!choice.tail) {
                    std::cout << "set choice.tail (via non-choice item).." << std::endl;
                    choice.tail = item;
                    if (!choice.last)
                        choice.last = item;
                }
                std::cout << "not choice, goto choice.head" << std::endl;
                this->goto_choice_head();
                continue;
            }
        }
        else {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
        }

        item = this->goto_next();
        if (choice.tail && item == choice.tail) {
            std::cout << "reached choice.tail, goto choice.head" << std::endl;
            this->goto_choice_head();
        }
        if (choice.last && !item) {
            std::cout << "out of items. goto choice.head" << std::endl;
            this->goto_choice_head();
        }
    } while(item != choice.last);

    // only one reason we are here:
    //      * there was no item matching str *in the choice*
    // but several reasons why that might be so,
    // so delay judgement for now...

    std::cout << "returning from choice with nothing!" << std::endl;

    return 0;
 }


 template <class T>
 bool item_list<T>::match_item_choice_is_valid()
 {
    llitem* li = choice.head;
    FLAGS previd = UI_DEFAULT;
    std::cout << "|+|+|+|+|+|+|+| MATCH_ITEM_CHOICE_IS_VALID |+|+|+|+|+|+|" << std::endl;
    do {
        base<T>* it = li->get_data();
        if (it->get_item_type() != UI_COMMENT) {
            FLAGS id = it->get_option_id();
            if (id == UI_DEFAULT || (id == UI_OPTION1 && previd > UI_OPTION1)) {
                if (!choice.tail) {
                    std::cout << "SETTING CHOICE.TAIL: "; it->dump();
                    choice.tail = it;
                }
                return true;
            }
            if (id != choice.opt0id && it->is_matched())
                return false;
            previd = id;
        }
        li = li->get_next();
    } while(li != 0);
    return true;
 }


 template <class T>
 base<T>* item_list<T>::match_item_chosen(const char* str)
 {
    std::cout << "=\nmatch_item_chosen(str:\"" << str << "\", item:'" << item->get_name() << "')\n=" << std::endl;

    if (choice.last == item) {
        if (!(item = this->goto_next()))
            this->goto_choice_head();
    }

    choice.dump();
    std::cout << "last: "; choice.last->dump();

    do {
        if (item->get_item_type() != UI_COMMENT) {

            #ifdef DEBUG
            std::cout << "   match_item_chosen ";
            item->dump();
            #endif

            FLAGS id = item->get_option_id();
            if (id & UI_OPTION_MASK) {
                if (!choice.tail) {
                    if (choice.opt0id > UI_OPTION1 && id == UI_OPTION1) {
                        std::cout << "set choice.tail; goto choice.head" << std::endl;
                        choice.tail = item;
                        this->goto_choice_head();
                        continue;
                    }
                }
                else if (item == choice.tail) {
                    if (item) {
                        std::cout << "item == choice.tail"; item->dump();
                    }
                    this->goto_choice_head();
                    continue;
                }
                if (id > choice.opt0id) {
                    choice.opt0 = this->sneak_current();
                    choice.opt0id = id;
                }

                if (item->is_name_match(str)) {
                    std::cout << "match_item_chosen about to validity test..." << std::endl;
                    if (id != choice.chosen0id) {
                        std::cout << "validity failed" << std::cout;
                        match_item_chosen_invalid_error();
                        return error_item<T>::err();
                    }
                    std::cout << "validity passed" << std::cout;
                    item->set_matched();
                    choice.chosen0 = choice.opt0;
                    choice.chosen0id = choice.opt0id;
                    choice.last = item;
                    return item;
                }
            }
            else {
                if (!choice.tail) {
                    std::cout << "set choice.tail (via non-choice item).." << std::endl;
                    choice.tail = item;
                    if (!choice.last)
                        choice.last = item;
                }
                std::cout << "not choice, goto choice.head" << std::endl;
                this->goto_choice_head();
                continue;
            }
        }
        else {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
        }

        item = this->goto_next();

        if (choice.tail && item == choice.tail) {
            std::cout << "reached choice.tail, goto choice.head" << std::endl;
            this->goto_choice_head();
        }
        if (choice.last && !item) {
            std::cout << "out of items. goto choice.head" << std::endl;
            this->goto_choice_head();
        }

    } while(item != choice.last);

    std::cout << "out of loop - - - item == choice.last" << std::endl;
    choice.dump();
    std::cout << "last: "; choice.last->dump();

    // only one reason we are here:
    //      * there was no item matching str *in the choice*
    // but several reasons why that might be so,
    // so delay judgement for now...

    return 0;
 }


 template <class T>
 void item_list<T>::match_item_chosen_invalid_error()
 {
    std::string errstr = "";
    int items = 0;

    llitem* li = choice.chosen0;

    do {
        base<T>* it = li->get_data();
        if (it->get_option_id() != choice.chosen0id)
            break;
        if (it->is_matched()) {
            errstr += (items == 0 ? "'" : ", '");
            errstr += it->get_name();
            errstr += "'";
            ++items;
        }
        li = li->get_next();
    } while (li);

    ui_err("Specifying item '%s' is invalidated by previous %s %s.",
            item->get_name(), (items > 1 ? "items" : "item"), errstr.c_str());
 }


 template <class T>
 base<T>* item_list<T>::validate_matches()
 {
    item = this->goto_first();
    std::cout << "validate_matches()" << std::endl;

    while (item) {
        if (item->get_item_type() == UI_COMMENT)
            item = this->goto_next();
        else {
            FLAGS id = item->get_option_id();
            if (id & UI_OPTION_MASK) {
                choice.reset();
                if (!validate_matches_choice())
                    return error_item<T>::err();
            }
            else {
                if (!item->is_matched()) {
                    if (item->is_optional()) {
                        if (*item == UI_INPUT) {
                            input_item<T>* in = static_cast<input_item<T>*>(item);
                            add_connector_off(subject, in->get_input_type());
                        }
                    }
                    else {
                        ui_err("Item '%s' is non-optional.", item->get_name());
                        return error_item<T>::err();
                    }
                }
            item = this->goto_next();
            }
        }
    }
    return 0;
 }


 template <class T>
 bool item_list<T>::validate_matches_choice()
 {
    // this does not need to test for invalid choice selection (ie
    // items matched from *both* choice 1 *and* choice 2) as that is
    // handled by the initial match_item process.

    bool choice_optional = item->is_optional();
    choice.head = choice.opt0 = this->sneak_current();
    choice.opt0id = item->get_option_id();

    // initial pass of choice selection, detects head
    // and chosen... and the no-choice-selected scenario
    // where selection of a choice is non-optional.

    while (item) {
        if (item->get_item_type() != UI_COMMENT) {
            FLAGS id = item->get_option_id();
            if ((choice.opt0id > UI_OPTION1 && id == UI_OPTION1)
             || id == UI_DEFAULT)
                break;
            if (id > choice.opt0id) {
                choice.opt0 = this->sneak_current();
                choice.opt0id = id;
            }
            if (item->is_matched() && !choice.chosen0) {
                choice.chosen0 = choice.opt0;
                break;
            }
        }
        item = this->goto_next();
    }

    if (choice_optional && !choice.chosen0) {
        // selection of the choice was optional and this fact
        // has been taken advantage of.  all that needs to be
        // done is to add off-connectors for any inputs in the
        // choice.
        item = this->goto_item(choice.head);
        while (item) {
            FLAGS id = item->get_option_id();
            if ((choice.opt0id > UI_OPTION1 && id == UI_OPTION1)
             || id == UI_DEFAULT)
                break;
            if (*item == UI_INPUT) {
                // add_connector_off is specialized.
                input_item<T>* in = static_cast<input_item<T>*>(item);
                add_connector_off(subject, in->get_input_type());
            }
            item = this->goto_next();
        }
        return true;
    }

    // second pass
    std::string errstr = "";
    std::string goodstr = "";
    int errors = 0;
    int goods = 0;

    item = this->goto_item(choice.chosen0);

    while (item) {
        FLAGS id = item->get_option_id();
        if ((choice.opt0id > UI_OPTION1 && id == UI_OPTION1)
         || id == UI_DEFAULT)
            break;
        if (item->is_matched()) {
            goodstr += (goods == 0 ? "'" : ", '");
            goodstr += item->get_name();
            goodstr += "'";
            ++goods;
        }
        else {
            if (!item->is_optional()) {
                errstr += (errors == 0 ? "'" : ", '");
                errstr += item->get_name();
                errstr += "'";
                ++errors;
            }
            else if (*item == UI_INPUT) {
                input_item<T>* in = static_cast<input_item<T>*>(item);
                add_connector_off(subject, in->get_input_type());
            }
        }
        item = this->goto_next();
    }

    if (errors) {
        ui_err("The following %s %s should have been specified "
               "alongside %s %s",
                (errors > 1 ? "items" : "item"), errstr.c_str(),
                (goods > 1 ? "items" : "item"), goodstr.c_str());
        return false;
    }

    return true;
 }


 template <class T>
 bool item_list<T>::data_validate(T t, param::TYPE pt, errors::TYPE et)
 {
    base<T>* i = this->goto_first();

    while(i) {
        if (*i == ui::UI_PARAM) {
            param_item<T>* p = static_cast<param_item<T>*>(i);
            if (*p == pt)
                return i->validate(t, et);
        }
        i = this->goto_next();
    }
    // failed to validate due to subject lacking parameter type
    return false;
 }


};

#endif
