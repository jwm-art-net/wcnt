#ifndef UI_ITEM_LIST_H#define UI_ITEM_LIST_H#include "connectorlist.h"#include "fxsparamlist.h"#include "textstuff.h"#include "ui_item.h"class connector;namespace wcnt {    fxsparamlist*   get_fxsparamlist();};namespace ui{ #ifdef DEBUG #define ui_err(fmt, ... )                              \ {                                                       \    cfmt(item_list<T>::err_msg, STRBUFLEN, "%40s:%5d %-35s\n" fmt, \                    __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);   \ } #else #define ui_err(fmt, ... ) \    cfmt(item_list<T>::err_msg, STRBUFLEN, fmt, __VA_ARGS__) #endif // T will be a pointer to synthmod::base or dobj::base template <class T> class item_list : public linked_list< base<T> > {  public:    item_list();    ~item_list() {};    base<T>* add_item(input::TYPE);    base<T>* add_item(param::TYPE);    base<T>* add_item(param::TYPE, const char* fixed_str);    base<T>* add_item(dobj::TYPE parent, dobj::TYPE child);    base<T>* add_item(const char* comment);    // specialist method, used by synthmod::base, dobj::base impl is empty.    connector* add_connector_off(T, input::TYPE);    // call match_begin at the start of each new module/data object to    // be read.    base<T>* match_begin(T);    base<T>* match_item(const char*);    // confirm all non-optional items have been matched etc,    // returns 0 on success or error<T>::err();    base<T>* match_validate();    // validates data for parameters, (T is the container)    bool data_validate(T, param::TYPE, errors::TYPE);  private:    T       subject;    enum { LIST_STD = 0xff, LIST_EDIT = 0xfff };    bool goodlist(int t) {        if (listtype == 0)            listtype = t;        return (listtype == t);    }    int listtype;    static char err_msg[STRBUFLEN]; }; template <class T> item_list<T>::item_list() : linked_list< base<T> >(MULTIREF_OFF), subject(0), listtype(0) {    std::cout << "item_list<T>::item_list<T>()" << std::endl;    std::cout << " + err_msg: " << (void*)err_msg << std::endl;    error_item<T>::err(err_msg); } template <class T> base<T>* item_list<T>::add_item(param::TYPE pt) {    if (pt == param::STR_UNNAMED || pt == param::STR_LIST) {        if (!goodlist(LIST_EDIT)) {            std::cout << "WE're *UNHAPPY AND WE KNOW IT!" << std::endl;            return 0;        }        else            std::cout << "WE ARE VERY HAPPY AND WE KNOW IT!" << std::endl;    }    else if (!goodlist(LIST_STD))        return 0;    base<T>* i = new param_item<T>(pt);    if (i) {        if (!this->add_at_tail(i)) {            delete i;            i = 0;        }    }    i->dump();    return i; } template <class T> base<T>* item_list<T>::add_item(param::TYPE pt, const char* fixstr) {    if (!goodlist(LIST_STD))        return 0;    if (!fixstr)        return 0;    base<T>* i = new param_item<T>(pt);    if (i) {        ll_item< base<T> > * li = this->add_at_tail(i);        if (li) {            if (!wcnt::get_fxsparamlist()->add_param(fixstr, pt)) {                delete this->unlink_item(li);                delete i;                i = 0;            }        }        else {            delete i;            i = 0;        }    }    return i; } template <class T> base<T>* item_list<T>::add_item(dobj::TYPE parent, dobj::TYPE child) {    if (!goodlist(LIST_STD))        return 0;    base<T>* i = new dobj_item<T>(parent, child);    if (i) {        if (!this->add_at_tail(i)) {            delete i;            i = 0;        }    }    return i; } template <class T> base<T>* item_list<T>::add_item(const char* comment) {    base<T>* i = new comment_item<T>(comment);    if (i) {        if (!this->add_at_tail(i)) {            delete i;            i = 0;        }    }    return i; } template <class T> base<T>* item_list<T>::match_begin(T t) {    std::cout << "\n--------------------------------------------------\n"              << "match_begin()" << std::endl;    subject = t;    base<T>* item = this->goto_first();    if (!item)        return 0;    int n = 0;    do {        item->reset_matched();        ll_item< base<T> >* li = this->sneak_current();        std::cout << "ll_item: " << (void*)li << "\titem: " << ++n << " - ";        item->dump();    } while((item = this->goto_next()) != 0);    return this->goto_first(); } template <class T> base<T>* item_list<T>::match_item(const char* str) {    if (!listtype || !str)        return 0;    std::cout << "match_item(" << str << ")" << std::endl;    ll_item< base<T> >* li = this->sneak_current();    std::cout << "ll_item: " << (void*)li<< std::endl;    base<T>* item = li->get_data();    if (listtype == LIST_EDIT) {        if (!item) {            std::cout << "**NOT ITEM**" << std::endl;            item = this->goto_first();        }        std::cout << "LIST_EDIT bypass matching proper!" << std::endl;        item->dump();        return item;    }    do {        #ifdef DEBUG        item->dump();        #endif        switch(item->get_item_type()) {          case UI_COMMENT:            std::cout << "skipping comment '"                      << item->get_descr() << "'" << std::endl;            continue;          case UI_PARAM: {            param_item<T>* p = static_cast<param_item<T>*>(item);            if (p->get_param_type() == param::STR_UNNAMED)                return item;          }          default:            break;        }        if (item->is_name_match(str)) {            if (item->is_matched()) {                std::cout << " FELELELELELELELEL" << std::endl;                ui_err("'%s' (0x%x) has already a value specified.", str, str);                return error_item<T>::err();            }            std::cout << "item match:" << (void*)item << std::endl;            item->set_matched();            return item;        }    } while((item = this->goto_next()) != 0); } template <class T> base<T>* item_list<T>::match_validate() {    if (listtype == LIST_EDIT) {        // no. i don't want to.        return 0;    }    FLAGS skip_id = UI_DEFAULT;    FLAGS match_id = UI_DEFAULT;    bool chosen = false;    bool choice_optional = false;    base<T>* prev = 0;    base<T>* choice = 0;    base<T>* group = 0;    base<T>* item = this->goto_first();    if (!item)        return 0;    std::cout << "\t++++++++++++++++++\nmatch_validate()" << std::endl;    FLAGS choiceid = UI_DEFAULT;    FLAGS groupid = UI_DEFAULT;    bool namechecked = false;    bool unmatched = false;    int itemno = 0; // item number within a group or choice    while(item) {        if (item->get_item_type() == UI_COMMENT) {            item = this->goto_next();            continue;        }        std::cout << "is valid "; item->dump();        FLAGS gid = item->get_group_id();        FLAGS cid = item->get_choice_id();        if (choice) {            if (cid & UI_CHOICE_MASK) {                if (prev && prev->get_choice_id() != cid)                    itemno = 1;                else                    ++itemno;                // continue the choice selection process                if (chosen) {                    if (cid == UI_CHOICE1                     && prev->get_choice_id() > UI_CHOICE1)                    {                        std::cout << "\t####1st choice item: '"; item->dump();                        choice = item;                        choice_optional = item->is_optional();                        chosen = item->is_matched();                        choiceid = (chosen ? cid : UI_DEFAULT);                        if (chosen) {                            std::cout << "\t++++chosen: '"; item->dump();}                    }                    else if (item->is_matched()) {                        std::cout << "\t++++matched: "; item->dump();                        if (cid > choiceid) {                            ui_err("item choice id mismatch %s",                                        "make the right choice!");                            return error_item<T>::err();                        }                    }                    else {                        ui_err("unmatched items in choice. %s",                                            "choose wisely!");                        return error_item<T>::err();                    }                }                else /* if (!chosen) */ {                    if (cid == UI_CHOICE1                     && prev->get_choice_id() > UI_CHOICE1)                    {                        ui_err("choice not made...%s",                                "choice you must make!");                        return error_item<T>::err();                    }                    if (item->is_matched()) {                        if (itemno != 1) {                            ui_err("unmatched items in choice. %s",                                        "choose wisely!");                            return error_item<T>::err();                        }                        chosen = true;                        choiceid = cid;                        if (chosen) {                            std::cout << "\t++++chosen: '"; item->dump();}                    }                }            }            else {                // end the choice selection process                if (chosen) {                    std::cout << "\tchoices end ("; item->dump();}                choice = 0;                if (!choice_optional && !chosen) {                    ui_err("you must choose... %s", "make a choice!");                    return error_item<T>::err();                }            }        }        if (!choice) {            if (cid & UI_CHOICE_MASK) {                // start the choice selection process                std::cout << "\t++++1st choice item: '"; item->dump();                choice = item;                itemno = 1;                choice_optional = item->is_optional();                chosen = item->is_matched();                choiceid = (chosen ? cid : UI_DEFAULT);                if (chosen)                    std::cout << "\t++++chosen: '"; item->dump();            }            else if (group) {            }            else /* (!group) */ {                if (gid & UI_GROUP_MASK) {                    group = item;                    groupid = (item->is_matched() ? gid : UI_DEFAULT);                }                else if (!item->is_matched() && !item->is_optional()) {                    ui_err("item '%s' must be specified", item->get_name());                    return error_item<T>::err();                }            }        }        if (!item->is_matched() && item->get_item_type() == UI_INPUT) {            input_item<T>* mi = static_cast<input_item<T>*>(item);            add_connector_off(subject, mi->get_input_type());        }        prev = item;        item = this->goto_next();    }    if (choice && !choice_optional && !chosen) {        ui_err("you must choose... %s", "make a choice!");        return error_item<T>::err();    }    // result!    return 0; } template <class T> bool item_list<T>::data_validate(T t, param::TYPE pt, errors::TYPE et) {    base<T>* i = this->goto_first();    while(i) {        if (*i == ui::UI_PARAM) {            param_item<T>* p = static_cast<param_item<T>*>(i);            if (*p == pt)                return i->validate(t, et);        }        i = this->goto_next();    }    // failed to validate due to subject lacking parameter type    return false; }};#endif