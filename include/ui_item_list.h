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

    base<T>* add_item(input::TYPE);  // <-- specialized
    base<T>* add_item(param::TYPE);
    base<T>* add_item(param::TYPE, const char* fixed_str);
    base<T>* add_item(dobj::TYPE parent, dobj::TYPE child);
    base<T>* add_item(const char* comment);

    // specialist method, used by synthmod::base, dobj::base impl is empty.
    connector* add_connector_off(T, input::TYPE);

    // call match_begin at the start of each new module/data object
    // definition being read from a file. match_edit at the start
    // of editing via param_editor etc.
    void     match_begin(T);
    void     match_edit(T);

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
    bool     editing;

    struct multiplechoice
    {
        llitem* head;       // first item of first option
        llitem* opt0;       // first item of current option
        llitem* chosen0;    // first item of chosen option
        base<T>* last;      // full circle (but not necessarily from start)
        FLAGS opt0id;
        FLAGS chosen0id;

        void reset() {
            head = opt0 = chosen0 = 0;
            last = 0;
            opt0id = chosen0id = UI_DEFAULT;
        }

        #ifdef DEBUG
        void dump() {
            std::cout << "option:\thead " << (void*) head;
            if (head)   std::cout << " ('" << head->get_data()->get_name() << "')";
            std::cout << "\topt0 " << (void*) opt0;
            if (opt0)   std::cout << " ('" << opt0->get_data()->get_name() << "')";
            std::cout << "\tchosen0 " << (void*) chosen0;
            if (chosen0)std::cout << " ('" << chosen0->get_data()->get_name() << "')";
            std::cout << std::endl;
            std::cout << "\tlast " << (void*) last;
            if (last)   std::cout << " ('" << last->get_name() << "')";
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

    void skip_choice();

    bool validate_matches_choice();
    bool validate_matches_group();

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
  item(0), prev(0), last(0), editing(false),
  listtype(0)
 {
    #ifdef DEBUG
    std::cout << "item_list<T>::item_list<T>()" << std::endl;
    std::cout << " + err_msg: " << (void*)err_msg << std::endl;
    #endif
    error_item<T>::err(err_msg);
 }

 template <class T>
 base<T>* item_list<T>::add_item(param::TYPE pt)
 {
    if (pt == param::STR_UNNAMED || pt == param::STR_LIST) {
        if (!goodlist(LIST_EDIT)) {
            return 0;
        }
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
    #ifdef DEBUG
    i->dump();
    #endif
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
    #ifdef DEBUG
    i->dump();
    #endif
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
    #ifdef DEBUG
    i->dump();
    #endif
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
    #ifdef DEBUG
    i->dump();
    #endif
    return i;
 }

 template <class T>
 void item_list<T>::match_begin(T t)
 {
    #ifdef DEBUG
    int n = 0;
    std::cout << "\n--------------------------------------------------\n"
              << "match_begin()" << std::endl;
    #endif

    editing = false;
    subject = t;

    if (!(item = this->goto_first()))
        return;

    do {
        item->reset_matched();
        #ifdef DEBUG
        llitem* li = this->sneak_current();
        std::cout << "ll_item: " << (void*)li << "\titem: " << ++n << " - ";
        item->dump();
        #endif
    } while ((item = this->goto_next()) != 0);

    choice.reset();

    prev = last = 0;
    item = this->goto_first();
 }


 template <class T>
 void item_list<T>::match_edit(T t)
 {
    editing = true;
    subject = t;
    choice.reset();
    prev = last = 0;
    item = this->goto_first();
 }

 template <class T>
 base<T>* item_list<T>::match_item(const char* str)
 {
    if (!listtype || !str || this->is_empty())
        return 0;

    #ifdef DEBUG
    std::cout << "\n\nmatch_item(" << str << ")" << std::endl;
    #endif

    base<T>* failchoice = 0;

restart:
    if (!item)
        item = this->goto_first();
    #ifdef DEBUG
    std::cout << "passed restart" << std::endl;
    std::cout << "item dump: "; item->dump();
    std::cout << "last dump: ";
    if (last)
        last->dump();
    else
        std::cout << "NULL" << std::endl;
    #endif

    if (last == item) {
        if (!(item = this->goto_next()))
            item = this->goto_first();
    }

    if (listtype == LIST_EDIT) {
        last = item;
        return item;
    }

    if (choice.opt0) {
        #ifdef DEBUG
        std::cout << "\t(CHOICE.CUR0)" << std::endl;
        #endif
        base<T>* i = match_item_choice(str);
        if (i)
            return i;
        failchoice = choice.head->get_data();
        #ifdef DEBUG
        std::cout << "\t*+*+*+*+*+*+*+ \tlast: "; last->dump();
        std::cout << "\t*+*+*+*+*+*+*+ \tchoice.last: "; choice.last->dump();
        std::cout << "\t*+*+*+*+*+*+*+ \tfailchoice: "; failchoice->dump();
        #endif
        skip_choice();
    }

    do {
        if (item->get_item_type() != UI_COMMENT) {

            #ifdef DEBUG
            std::cout << "match_item loop on: ";
            item->dump();
            #endif

            FLAGS id = item->get_option_id();

            if (id & UI_OPTION_MASK) {
                if (item == failchoice) {
                    #ifdef DEBUG
                    std::cout << "\n\tFAILCHOICE BREAK 1\n" << std::endl;
                    #endif
                    break;
                }
                choice.head = choice.opt0 = this->sneak_current();
                choice.opt0id = id;
                choice.chosen0 = 0;
                choice.chosen0id = UI_DEFAULT;
                choice.last = 0;
                #ifdef DEBUG
                std::cout << "/////////// choice.head llitem: " << (void*) choice.head << std::endl;
                #endif
                base<T>* ic = match_item_choice(str);
                if (ic)
                    return ic;
                if (!failchoice) {
                    failchoice = choice.head->get_data();
                    #ifdef DEBUG
                    std::cout << "\n\t*+*+*+*+*+*+*+ \tfailchoice: "; failchoice->dump();
                    #endif
                }
                #ifdef DEBUG
                std::cout << "resetting choice before restart..." << std::endl;
                #endif
                skip_choice();
                if (item == failchoice) {
                    #ifdef DEBUG
                    std::cout << "\n\tFAILCHOICE BREAK 2\n" << std::endl;
                    #endif
                    break;
                }
                goto restart;
            }

            if (item->is_name_match(str)) {
                if (!editing && item->is_matched()) {
                    ui_err("Duplicate item: %s.", item->get_name());
                    return error_item<T>::err();
                }
                #ifdef DEBUG
                std::cout << "item matched, not choice/group" << std::endl;
                #endif
                item->set_matched();
                last = item;
                return item;
            }
        }
        #ifdef DEBUG
        else {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
        }
        #endif

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
    char buf[80];
    size_t l = strlen(str);

    if (l > 80) {
        strncpy(buf, str, 80);
        buf[79] = '\0';
        l = 79;
    }
    else
        strcpy(buf, str);

    bool forced = false;

    if (buf[l - 1] == '!') {
        buf[l - 1] = '\0';
        forced = true;
    }

    str = buf;

    #ifdef DEBUG
    std::cout << "match_item_choice(str:\"" << str << "\", item:'" << item->get_name() << "')" << std::endl;
    std::cout << "-------------------- choice.head llitem: " << (void*) choice.head << std::endl;
    #endif

    if (!choice.last)
        choice.last = item;
    else if (choice.last == item) {
        if (!(item = this->goto_next()))
            this->goto_choice_head();
        //choice.last = item;
    }
    #ifdef DEBUG
    std::cout << "\t!^!^!^!^!^!^!^ \tchoice.last: ";
    if (choice.last) choice.last->dump(); else std::cout << "NULL" << std::endl;
    #endif

    do {
        if (item->get_item_type() != UI_COMMENT) {
            #ifdef DEBUG
            std::cout << "   match_item_choice ";
            item->dump();
            #endif

            FLAGS id = item->get_option_id();

            if (id & UI_OPTION_MASK) {
                if (choice.opt0id > UI_OPTION1 && id == UI_OPTION1) {
                    #ifdef DEBUG
                    std::cout << "\n\t*1*CHOICETAIL == ITEM (" << (void*) item << ")" << std::endl;
                    #endif
                    this->goto_choice_head();
                    continue;
                }
                if (id > choice.opt0id) {
                    choice.opt0 = this->sneak_current();
                    choice.opt0id = id;
                }

                if (!item->is_dummy()) {
                    if (!choice.chosen0) {
                        if (item->is_matched() && !item->was_forced()) {
                            #ifdef DEBUG
                            std::cout << "detected previously matched choice item" << std::endl;
                            #endif
                            choice.chosen0 = choice.opt0;
                            choice.chosen0id = choice.opt0id;
                        }
                    }

                    if (item->is_name_match(str)) {
                        if (!editing && item->is_matched()) {
                            ui_err("Item '%s' already specified.", item->get_name());
                            return error_item<T>::err();
                        }
                        if (forced)
                            item->set_forced();
                        else {
                            if (!choice.chosen0) {
                                choice.chosen0 = choice.opt0;
                                choice.chosen0id = choice.opt0id;
                            }
                            else if (id != choice.chosen0id) {
                                #ifdef DEBUG
                                std::cout << "validity failed" << std::cout;
                                #endif
                                match_item_chosen_invalid_error();
                                return error_item<T>::err();
                            }
                        }
                        item->set_matched();
                        choice.last = item;
                        last = choice.head->get_data();
                        return item;
                    }
                }
            }
            else {
                #ifdef DEBUG
                std::cout << "\n\t*2*CHOICETAIL == ITEM (" << (void*) item << ")" << std::endl;
                #endif
                this->goto_choice_head();
                continue;
            }
        }
        #ifdef DEBUG
        else {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
        }
        #endif

        item = this->goto_next();

        if (choice.last && !item) {
            #ifdef DEBUG
            std::cout << "out of items. goto choice.head" << std::endl;
            #endif
            this->goto_choice_head();
        }
    } while(item != choice.last);

    // only one reason we are here:
    //      * there was no item matching str *in the choice*
    // but several reasons why that might be so,
    // so delay judgement for now...
    #ifdef DEBUG
    std::cout << "returning from choice with nothing!" << std::endl;
    #endif

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
        if (item->get_item_type() != UI_COMMENT && !item->is_dummy()) {
            if (it->get_option_id() != choice.chosen0id)
                break;
            if (it->is_matched()) {
                #ifdef DEBUG
                std::cout << "adding error item:'" << it->get_name() << "'" << std::endl;
                #endif
                errstr += (items == 0 ? "'" : ", '");
                errstr += it->get_name();
                errstr += "'";
                ++items;
            }
        }
        li = li->get_next();
    } while (li);

    ui_err("Specifying item '%s' is invalidated by previous selection of %s %s.",
            item->get_name(), (items > 1 ? "items" : "item"), errstr.c_str());
 }


 template <class T>
 base<T>* item_list<T>::validate_matches()
 {
    if (listtype == LIST_EDIT)
        return 0;

    item = this->goto_first();
    #ifdef DEBUG
    std::cout << "validate_matches()" << std::endl;
    #endif

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
            else if ((id = item->get_group_id()) & UI_GROUP_MASK) {
                if (!validate_matches_group())
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
                choice.chosen0id = choice.opt0id;
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
            if (item->get_item_type() != UI_COMMENT) {
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
        #ifdef DEBUG
        std::cout << "validate ";
        item->dump();
        #endif
        if (item->get_item_type() != UI_COMMENT) {
            FLAGS id = item->get_option_id();
            if (id != choice.chosen0id)
                break;
            #ifdef DEBUG
            std::cout << "processing..." << std::endl;
            #endif
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
 bool item_list<T>::validate_matches_group()
 {
    bool selected = false;
    bool unmatched = false;
    llitem* group0 = this->sneak_current();
    FLAGS group0id = item->get_group_id();

    #ifdef DEBUG
    std::cout << "validate group\n";
    #endif

    while (item) {
        #ifdef DEBUG
        std::cout << "validate... ";
        item->dump();
        #endif
        if (item->get_item_type() != UI_COMMENT) {
            #ifdef DEBUG
            std::cout << "processing...";
            #endif
            if (item->get_group_id() != group0id)
                break;
            if (item->is_matched()) {
                selected = true;
                #ifdef DEBUG
                std::cout << "selected" << std::endl;
                #endif
            }
            else {
                if (!item->is_optional()) {
                    unmatched = true;
                    #ifdef DEBUG
                    std::cout << "unmatched" << std::endl;
                    #endif
                }
                if (*item == UI_INPUT) {
                    #ifdef DEBUG
                    std::cout << "adding off connector..." << std::endl;
                    #endif
                    // optimize for non-error situation
                    input_item<T>* in = static_cast<input_item<T>*>(item);
                    add_connector_off(subject, in->get_input_type());
                }
            }
        }
        item = this->goto_next();
    }

    if (!selected || (selected && !unmatched))
        return true;


    std::string errstr = "";
    std::string goodstr = "";
    int errors = 0;
    int goods = 0;

    item = this->goto_item(group0);

    while (item) {
        if (item->get_item_type() != UI_COMMENT) {
            if (item->get_group_id() != group0id)
                break;
            if (item->is_matched()) {
                goodstr += (goods == 0 ? "'" : ", '");
                goodstr += item->get_name();
                goodstr += "'";
                ++goods;
            }
            else if (!item->is_optional()) {
                errstr += (errors == 0 ? "'" : ", '");
                errstr += item->get_name();
                errstr += "'";
                ++errors;
            }
        }
        item = this->goto_next();
    }

    ui_err("The following %s %s should have been specified alongside %s %s",
            (errors > 1 ? "items" : "item"), errstr.c_str(),
            (goods > 1 ? "items" : "item"), goodstr.c_str());
    return false;
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

 template <class T>
 void item_list<T>::skip_choice()
 {
    FLAGS cid = item->get_option_id();
    bool wrap = false;
    #ifdef DEBUG
    std::cout << "\t\t\tSKIPPING CHOICE (";
    #endif
    do {
        if (item->get_item_type() != UI_COMMENT) {
            if (wrap && this->sneak_current() == choice.head)
                break;
            FLAGS id = item->get_option_id();
            if ((id == UI_OPTION1 && cid > UI_OPTION1)
              || id == UI_DEFAULT)
                break;
            cid = id;
            #ifdef DEBUG
            std::cout << "'" << item->get_name() << "' ";
            #endif
        }
        if (!(item = this->goto_next())) {
            item = this->goto_first();
            wrap = true;
            break;
        }
    } while (true);

    #ifdef DEBUG
    std::cout << ")" << std::endl;
    #endif
    choice.reset();
 }


};

#endif
