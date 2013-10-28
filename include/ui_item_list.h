#ifndef UI_ITEM_LIST_H
#define UI_ITEM_LIST_H

#include "connectorlist.h"
#include "fxsparamlist.h"
#include "textstuff.h"
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

    base<T>* add_item(input::TYPE);
    base<T>* add_item(param::TYPE);
    base<T>* add_item(param::TYPE, const char* fixed_str);
    base<T>* add_item(dobj::TYPE parent, dobj::TYPE child);
    base<T>* add_item(const char* comment);

    bool validate(T, param::TYPE, errors::TYPE);

    connector* add_connector_off(T, input::TYPE);

    base<T>* match_begin(T);
    base<T>* match_item(const char*);

  private:
    FLAGS   skip_id;
    FLAGS   match_id;
    base<T>* match_prev;
    base<T>* choice;
    bool    chosen;
    bool    choice_optional;
    T       subject;
    static char err_msg[STRBUFLEN];
 };


 template <class T>
 item_list<T>::item_list() :
 linked_list< base<T> >(MULTIREF_OFF),
 skip_id(UI_DEFAULT), match_id(UI_DEFAULT), match_prev(0),
 choice(0), chosen(false), choice_optional(false),
 subject(0)
 {
    std::cout << "item_list<T>::item_list<T>()" << std::endl;
    std::cout << " + err_msg: " << (void*)err_msg << std::endl;
    error_item<T>::err(err_msg);
 }

 template <class T>
 base<T>* item_list<T>::add_item(input::TYPE it)
 {
    base<T>* i = new input_item<T>(it);
    if (i) {
        if (!linked_list< base<T> >::add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 template <class T>
 base<T>* item_list<T>::add_item(param::TYPE pt)
 {
    base<T>* i = new param_item<T>(pt);
    if (i) {
        if (!linked_list< base<T> >::add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 template <class T>
 base<T>* item_list<T>::add_item(param::TYPE pt, const char* fixstr)
 {
    if (!fixstr)
        return 0;

    base<T>* i = new param_item<T>(pt);
    if (i) {
        ll_item< base<T> > * li = linked_list< base<T> >::add_at_tail(i);
        if (li) {
            if (!wcnt::get_fxsparamlist()->add_param(fixstr, pt)) {
                delete linked_list< base<T> >::unlink_item(li);
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
    base<T>* i = new dobj_item<T>(parent, child);
    if (i) {
        if (!linked_list< base<T> >::add_at_tail(i)) {
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
        if (!linked_list< base<T> >::add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 template <class T>
 bool item_list<T>::validate(T t, param::TYPE pt, errors::TYPE et)
 {
    base<T>* i = linked_list< base<T> >::goto_first();

    while(i) {
        if (*i == ui::UI_PARAM) {
            param_item<T>* p = static_cast<param_item<T>*>(i);
            if (*p == pt)
                return i->validate(t, et);
        }
        i = linked_list< base<T> >::goto_next();
    }
    // failed to validate due to subject lacking parameter type
    return false;
 }

 template <class T>
 base<T>* item_list<T>::match_begin(T t)
 {
    subject = t;
    match_prev = 0;
    skip_id = match_id = UI_DEFAULT;
    choice = 0;
    chosen = false;
    choice_optional = false;
    return linked_list< base<T> >::goto_first();
 }

 template <class T>
 base<T>* item_list<T>::match_item(const char* str)
 {
    if (!str)
        return 0;

    ll_item< base<T> >* li = linked_list< base<T> >::sneak_current();

    if (!li)
        return 0;

    base<T>* item = li->get_data();
    bool namechecked = false;

    do {
        if (item->get_item_type() == UI_COMMENT) {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
            continue;
        }
        #ifdef DEBUG
        item->dump();
        #endif
        if (match_prev) {
            if (match_prev->get_set_id() != item->get_set_id())
            {
                std::cout << "resetting skip/match id " << std::endl;
                skip_id = match_id = UI_DEFAULT;
            }
        }

        base<T>* prev = match_prev;
        match_prev = item;
        FLAGS setid = item->get_set_id();
        FLAGS choiceid = item->get_choice_id();

        if (!choice) {
            std::cout <<"\t----- if (!choice" << std::endl;
            if ((choiceid & UI_CHOICE_MASK)) {
                choice = item;
                chosen = false;
                choice_optional = item->is_optional();
                std::cout << "\t|||||| 1st choice..." << std::endl;
                std::cout << "\t|||||| " << item->get_name() << std::endl;
            }
        }
        else { // if (choice) {
            std::cout <<"\t+++++ if (choice" << std::endl;
            if (!(choiceid & UI_CHOICE_MASK)) {
                // not a choice item, set choice state off
                if (!choice_optional && !chosen) {
                    // none of the choices were taken but is that ok?
                    ui_err("must %s", "make a choice.");
                    return error_item<T>::err();
                }
                choice = 0;
            }
        }

        int ret = item->is_match(str, skip_id, match_id);

        switch(ret) {
          case ITEM_SKIPPED:
            if (!namechecked) {
                const char* uname = subject->get_username();
                if ((uname && strcmp(str, uname)) != 0
                 && param::names::type(str) == param::ERR_TYPE
                 && dobj::names::type(str) == dobj::ERR_TYPE
                 && input::names::type(str) == input::ERR_TYPE)
                {
                    ui_err("'%s' is not recognised as a parameter name, "
                             "data object name, nor input name.", str);
                    return error_item<T>::err();
                }
                namechecked = true;
            }
            break;
          case ITEM_MATCH:
            if (setid != UI_DEFAULT) {
                std::cout << "====== setting set..." << std::endl;
                std::cout << "====== set id: " << setid << std::endl;
                match_id = setid;
            }
            else if (choice != 0) {
                if (!chosen) {
                    skip_id = UI_CHOICE_MASK;
                    match_id = item->get_choice_id();
                    std::cout << "_____________________choice/match id: "
                              << match_id << std::endl;
                    chosen = true;
                }
            }
            return item;
          case ITEM_UNEXPECTED:
            ui_err("unexpected '%s', expected '%s'.", str, item->get_name());
            return error_item<T>::err();
          case ITEM_SET_EXPECTED:
            ui_err("'%s' must be specified after '%s'.", item->get_name(),
                                                           prev->get_name());
            return error_item<T>::err();
          case ITEM_SET_UNEXPECTED:
            ui_err("'%s' should not be specified when '%s' was not.",
                                         item->get_name(), prev->get_name());
            return error_item<T>::err();
          case ITEM_CHOICE_EXPECTED:
            ui_err("'%s' must be specified after '%s' " //****
                     "(not as specified '%s').", item->get_name(),
                                                 prev->get_name(), str);
            return error_item<T>::err();
          case ITEM_CHOICE_UNEXPECTED:
            ui_err("'%s' should not be specified after '%s'.",
                                         item->get_name(), choice->get_name());
            return error_item<T>::err();
          default:
            ui_err("%s unhandled ui item match return code (%d)",
                                            errors::stock::major, ret);
            return error_item<T>::err();
            break;
        }

        //  item was skipped...
        if (setid != UI_DEFAULT)
            skip_id = setid;

        // if it is an input then we must create
        //  a connector to ensure it is properly turned off...
        if (item->get_item_type() == UI_INPUT) {
            input_item<T>* mi = static_cast<input_item<T>*>(item);
            add_connector_off(subject, mi->get_input_type());
        }

    } while((item = linked_list< base<T> >::goto_next()) != 0);

    std::cout << "no more items.." << std::endl;
    return 0;
 }



};

#endif
