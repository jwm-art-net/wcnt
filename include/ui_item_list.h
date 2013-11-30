#ifndef UI_ITEM_LIST_H
#define UI_ITEM_LIST_H

#include "connectorlist.h"
#include "fxsparamlist.h"
#include "textstuff.h"
#include "types.h"
#include "ui_item.h"

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

    /*
    // confirm all non-optional items have been matched etc,
    // returns 0 on success or error<T>::err();
    base<T>* match_validate();
    */

    void validate_matches();

    // validates data for parameters, (T is the container)
    bool data_validate(T, param::TYPE, errors::TYPE);

  private:
    T       subject;

    FLAGS    skip_id;
    FLAGS    match_id;
    base<T>* item;
    base<T>* prev;
    base<T>* last;
/*
    llitem*  choice0;    // first item within current choice (n of 4)
    FLAGS    choice0id;  // id (UI_CHOICEn)
    llitem*  chosen0;    // first item of chosen choice (n of 4)
    FLAGS    chosen0id;  // ...
    base<T>* choicelast;
*/
    llitem*  group0;
    FLAGS    group0id;
    base<T>* grouplast;

    struct choicedata
    {
        llitem* head;       // first item of first choice
        llitem* cur0;       // first item of current choice
        llitem* chosen0;    // first item of chosen choice
        base<T>* prev;      // previous scanned item;
        base<T>* last;      // gone full circle
        base<T>* tail;      // last item of last choice
        FLAGS cur0id;
        FLAGS chosen0id;

        void reset() {
            head = cur0 = chosen0 = 0;
            prev = last = tail = 0;
            cur0id = chosen0id = UI_DEFAULT;
        }

    } choice;


    base<T>* match_item_choice(const char*);
    bool     match_item_choice_is_valid();
    base<T>* match_item_chosen(const char*);
    bool     match_item_choice_all_present();

/*
    bool        end_choice_check();
    base<T>*    match_item_group(const char*, base<T>*);
*/

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
 /*choice0(0), chosen0(0), chosen0id(UI_DEFAULT),*/
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
    llitem* li = this->sneak_first();
    do {
        base<T>* it = li->get_data();
        if (it->is_name_match(str))
            return true;
        li = li->get_next();
    } while (li);
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

restart:
    std::cout << "passed restart" << std::endl;

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
        choice.reset();
    }
    if (choice.cur0) {
        std::cout << "\t(CHOICE.CUR0)" << std::endl;
        base<T>* ic = match_item_choice(str);
        if (ic)
            return ic;
        choice.reset();
    }
/*    else if (group0)
        return match_item_group(str, item);
*/
    do {
        if (item->get_item_type() != UI_COMMENT) {

            FLAGS id = item->get_choice_id();

            if (id & UI_CHOICE_MASK) {
                choice.chosen0 = 0;
                choice.prev = choice.last = choice.tail = 0;
                choice.head = choice.cur0 = this->sneak_current();
                choice.cur0id = id;
                choice.chosen0id = UI_DEFAULT;
                std::cout << "/////////// choice.head llitem: " << (void*) choice.head << std::endl;
                base<T>* ic = match_item_choice(str);
                if (ic)
                    return ic;
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
            item = this->goto_item(choice.head);
    }

    do {
        if (item->get_item_type() != UI_COMMENT) {

            #ifdef DEBUG
            std::cout << "   match_item_choice ";
            item->dump();
            #endif

            FLAGS id = item->get_choice_id();
            if (id & UI_CHOICE_MASK) {
                if (!choice.tail) {
                    if (choice.cur0id > UI_CHOICE1 && id == UI_CHOICE1) {
                        choice.tail = item;
                        item = this->goto_item(choice.head);
                        continue;
                    }
                }
                else if (item == choice.tail) {
                    item = this->goto_item(choice.head);
                    continue;
                }
                if (id > choice.cur0id) {
                    choice.cur0 = this->sneak_current();
                    choice.cur0id = id;
                }

                if (item->is_name_match(str)) {
                    if (item->is_matched()) {
                        ui_err("item '%s' already specified.", item->get_name());
                        return error_item<T>::err();
                    }
                    if (!match_item_choice_is_valid()) {
                        std::cout << "match item choice is invalid" << std::endl;
                        ui_err("invalid item after previously %s",
                                        "selecting choice item");
                        return error_item<T>::err();
                    }
                    item->set_matched();
                    choice.chosen0 = choice.cur0;
                    choice.chosen0id = choice.cur0id;
                    choice.last = item;
                    return item;
                }
            }
            else
                item = this->goto_item(choice.head);
        }
        else {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
        }

        item = this->goto_next();
        if (choice.last && !item)
            item = this->goto_item(choice.head);

    } while(item != choice.last);

    // only one reason we are here:
    //      * there was no item matching str in the choice *
    // but several reasons why that might be so.

    return 0;
 }


 template <class T>
 bool item_list<T>::match_item_choice_is_valid()
 {
    llitem* li = choice.head;
    FLAGS previd = UI_DEFAULT;
    do {
        base<T>* it = li->get_data();
        FLAGS id = it->get_choice_id();
        if (id == UI_DEFAULT || (id == UI_CHOICE1 && previd > UI_CHOICE1)) {
            if (!choice.tail)
                choice.tail = it;
            return true;
        }
        if (id != choice.cur0id && it->is_matched())
            return false;
        li = li->get_next();
        previd = id;
    } while(li != 0);
    return true;
 }

 template <class T>
 base<T>* item_list<T>::match_item_chosen(const char* str)
 {
    std::cout << "match_item_chosen(str:\"" << str << "\", item:'" << item->get_name() << "')" << std::endl;

    if (choice.last == item) {
        if (!(item = this->goto_next()))
            item = this->goto_item(choice.head);
    }

    do {
        if (item->get_item_type() != UI_COMMENT) {

            #ifdef DEBUG
            std::cout << "   match_item_chosen ";
            item->dump();
            #endif

            FLAGS id = item->get_choice_id();
            if (id & UI_CHOICE_MASK) {
                if (!choice.tail) {
                    if (choice.cur0id > UI_CHOICE1 && id == UI_CHOICE1) {
                        choice.tail = item;
                        item = this->goto_item(choice.head);
                        continue;
                    }
                }
                else if (item == choice.tail) {
                    item = this->goto_item(choice.head);
                    continue;
                }
                {
                    if (id > choice.cur0id) {
                        choice.cur0 = this->sneak_current();
                        choice.cur0id = id;
                    }

                    if (item->is_name_match(str)) {
                        std::cout << "match_item_chosen about to validity test..." << std::endl;
                        if (id != choice.chosen0id) {
                            std::cout << "validity failed" << std::cout;
                            ui_err("invalid item after previously %s",
                                            "selecting choice item");
                            return error_item<T>::err();
                        }
                        std::cout << "validity passed" << std::cout;
                        item->set_matched();
                        choice.chosen0 = choice.cur0;
                        choice.chosen0id = choice.cur0id;
                        return item;
                    }
                }
            }
            else
                item = this->goto_item(choice.head);
        }
        else {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
        }

        item = this->goto_next();
        if (choice.last && !item)
            item = this->goto_item(choice.head);

    } while(item != choice.last);

    // only one reason we are here:
    //      * there was no item matching str in the choice *
    // but several reasons why that might be so.

    if (!quick_scan(str)) {
        ui_err("no item found matching '%s'.", str);
        return error_item<T>::err();
    }

    if (!match_item_choice_all_present())
        return error_item<T>::err();

    return 0;
 }

 template <class T>
 bool item_list<T>::match_item_choice_all_present()
 {
    char buf_a[STRBUFLEN] = "The following non-optional items were missing:";
    char buf_b[STRBUFLEN] = "";
    char* b1 = buf_a;
    char* b2 = buf_b;
    llitem* li = choice.chosen0;
    base<T>* it;
    int items = 0;
    int errors = 0;
    do {
        ++items;
        it = li->get_data();
        if (!it->is_optional() && !it->is_matched()) {
            cfmt(b2, STRBUFLEN, (errors > 0 ? "%s, %s" : "%s %s"), b1, it->get_name());
            b1 = (b1 == buf_a ? buf_b : buf_a);
            b2 = (b2 == buf_b ? buf_a : buf_b);
            ++errors;
        }
        li = li->get_next();
    } while (li && it->get_choice_id() == choice.chosen0id);

    if (errors) {
        ui_err("%s", b1);
        return false;
    }

    return true;
 }


/*
        std::cout << "-x-x-x-x- within choice state -x-x-x-x- " << std::endl;
        if (cid == choice0id) {
            std::cout << "-x-x-x- cid == choice0id -x-x-x-" << std::endl;
            if (matched) {
                std::cout << "-x-x-x- matched -x-x-x-" << std::endl;
                if (!chosen0) {
                    std::cout << "chosen item (choice0): "; choice0->get_data()->dump();
                    chosen0 = choice0;
                }
                else if ((chosen0id & UI_CHOICE_MASK)
                      && chosen0id != cid)
                {
                    std::cout << "-x-x-x- chosen0id != cid -x-x-x-" << std::endl;
                    ui_err("choice already made. %s", "no turning back now!");
                    return error_item<T>::err();
                }
                std::cout << "-x-x-x- OK returning item -x-x-x-" << std::endl;
                item->set_matched();
                return item;
            }
        }
        else { // next or new choice state id
            std::cout << "-x-x-x- new choice: "; item->dump();

            if (chosen0) {
                std::cout << "-x-x-x- chosen0 -x-x-x-" << std::endl;
                if (!this->end_choice_check()) {item->dump();
                    std::cout << "-x-x-x- end_choice_check fail :-( -x-x-x-" << std::endl;
                    // ui_err() called by end_choice_check
                    //ui_err("choice items unspecified. %s",
                      //          "please choose wisely!");
                    return error_item<T>::err();
                }
            }
            if (cid & UI_CHOICE_MASK) {
                choice0 = this->sneak_current();
                choice0id = cid;
                goto retest;
            }
        }
*/




/*

 template <class T>
 bool item_list<T>::end_choice_check()
 {  // this function should not be called until either a new choice item
    // is reached, or a non-choice item is reached. and then it should only
    // be called after choice_match_item.()

    bool err = false;
    char tmp1[STRBUFLEN] = { "" };
    char tmp2[STRBUFLEN] = { "the following non-optional items were not specified:" };

    std::cout << "end choice check..." << std::endl;
    std::cout << "----------------..." << std::endl;

    std::cout << "choice0id:" << std::hex << choice0id << std::endl;

    llitem* li = choice0;
    do {
        base<T>* item = li->get_data();
        std::cout << "endchoice checking>>"; item->dump();
        if (item->get_choice_id() != choice0id) {
            std::cout << "breaking choice check on item ";
            item->dump();
            break;
        }
        if (!item->is_matched() && !item->is_optional()) {
            std::cout << "oops";
            cfmt(tmp1, STRBUFLEN, "%s %s", tmp2, item->get_name());
            cfmt(tmp2, STRBUFLEN, "%s", tmp1);
            err = true;
        }
    } while ((li = li->get_next()));

    if (err) {
        ui_err("%s", tmp2);
        return false;
    }

    chosen0id = choice0id;
    return true;
 }

 template <class T>
 base<T>* item_list<T>::match_item_chosen(const char* str, base<T>* item)
 {
 }




 template <class T>
 base<T>* item_list<T>::match_item_group(const char* str, base<T>* item)
 {
    std::cout << "match_item_choice(str:\"" << str << "\", item:'" << item->get_name() << "')" << std::endl;

    do {
    return 0;
 }
*/

/*
 template <class T>
 base<T>* item_list<T>::match_validate()
 {
    if (listtype == LIST_EDIT) {
        // no. i don't want to.
        return 0;
    }

    FLAGS skip_id = UI_DEFAULT;
    FLAGS match_id = UI_DEFAULT;
    bool chosen = false;
    bool choice_optional = false;
    base<T>* prev = 0;
    base<T>* choice = 0;
    base<T>* group = 0;
    base<T>* item = this->goto_first();

    if (!item)
        return 0;

    std::cout << "\t++++++++++++++++++\nmatch_validate()" << std::endl;

    FLAGS choiceid = UI_DEFAULT;
    FLAGS groupid = UI_DEFAULT;

    bool namechecked = false;
    bool unmatched = false;
    int itemno = 0; // item number within a group or choice

    while(item) {
        if (item->get_item_type() == UI_COMMENT) {
            item = this->goto_next();
            continue;
        }

        std::cout << "is valid "; item->dump();

        FLAGS gid = item->get_group_id();
        FLAGS cid = item->get_choice_id();

        if (choice) {
            if (cid & UI_CHOICE_MASK) {
                if (prev && prev->get_choice_id() != cid)
                    itemno = 1;
                else
                    ++itemno;
                // continue the choice selection process
                if (chosen) {
                    if (cid == UI_CHOICE1
                     && prev->get_choice_id() > UI_CHOICE1)
                    {
                        std::cout << "\t####1st choice item: '"; item->dump();
                        choice = item;
                        choice_optional = item->is_optional();
                        chosen = item->is_matched();
                        choiceid = (chosen ? cid : UI_DEFAULT);
                        if (chosen) {
                            std::cout << "\t++++chosen: '"; item->dump();}
                    }
                    else if (item->is_matched()) {
                        std::cout << "\t++++matched: "; item->dump();
                        if (cid > choiceid) {
                            ui_err("item choice id mismatch %s",
                                        "make the right choice!");
                            return error_item<T>::err();
                        }
                    }
                    else {
                        ui_err("unmatched items in choice. %s",
                                            "choose wisely!");
                        return error_item<T>::err();
                    }
                }
                else { // if (!chosen)
                    if (cid == UI_CHOICE1
                     && prev->get_choice_id() > UI_CHOICE1)
                    {
                        ui_err("choice not made...%s",
                                "choice you must make!");
                        return error_item<T>::err();
                    }

                    if (item->is_matched()) {
                        if (itemno != 1) {
                            ui_err("unmatched items in choice. %s",
                                        "choose wisely!");
                            return error_item<T>::err();
                        }
                        chosen = true;
                        choiceid = cid;
                        if (chosen) {
                            std::cout << "\t++++chosen: '"; item->dump();}
                    }
                }
            }
            else {
                // end the choice selection process
                if (chosen) {
                    std::cout << "\tchoices end ("; item->dump();}
                choice = 0;
                if (!choice_optional && !chosen) {
                    ui_err("you must choose... %s", "make a choice!");
                    return error_item<T>::err();
                }
            }
        }

        if (!choice) {
            if (cid & UI_CHOICE_MASK) {
                // start the choice selection process
                std::cout << "\t++++1st choice item: '"; item->dump();
                choice = item;
                itemno = 1;
                choice_optional = item->is_optional();
                chosen = item->is_matched();
                choiceid = (chosen ? cid : UI_DEFAULT);
                if (chosen)
                    std::cout << "\t++++chosen: '"; item->dump();
            }
            else if (group) {

            }
            else {// (!group) {
                if (gid & UI_GROUP_MASK) {
                    group = item;
                    groupid = (item->is_matched() ? gid : UI_DEFAULT);
                }
                else if (!item->is_matched() && !item->is_optional()) {
                    ui_err("item '%s' must be specified", item->get_name());
                    return error_item<T>::err();
                }
            }
        }

        if (!item->is_matched() && item->get_item_type() == UI_INPUT) {
            input_item<T>* mi = static_cast<input_item<T>*>(item);
            add_connector_off(subject, mi->get_input_type());
        }

        prev = item;
        item = this->goto_next();
    }

    if (choice && !choice_optional && !chosen) {
        ui_err("you must choose... %s", "make a choice!");
        return error_item<T>::err();
    }

    // result!
    return 0;
 }
*/

 template <class T>
 void item_list<T>::validate_matches()
 {
    base<T>* i = this->goto_first();

    std::cout << "validate_matches()" << std::endl;

    while(i) {
        if (*i == ui::UI_INPUT) {
            i->dump();
            input_item<T>* in = static_cast<input_item<T>*>(i);
            if (!in->is_matched() && in->is_optional()) {
                std::cout << "adding connector." << std::endl;
                add_connector_off(subject, in->get_input_type());
            }
        }
        i = this->goto_next();
    }
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
