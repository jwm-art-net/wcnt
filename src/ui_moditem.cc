#include "../include/ui_moditem.h"
#include "../include/checkvalue.h"
#include "../include/fxsparamlist.h"
#include "../include/globals.h"
#include "../include/listwork.h"
#include "../include/connectorlist.h"

#include <iostream>


namespace ui
{

 #ifdef DEBUG
 #define uimoderr(fmt, ... )                              \
 {                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(moditem_list::err_msg, STRBUFLEN, fmt, __VA_ARGS__);   \
 }
 #else
 #define uimoderr(fmt, ... ) \
    cfmt(moditem_list::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
 #endif

 char moditem_list::err_msg[STRBUFLEN] = "";

 moditem_list::moditem_list() :
 linkedlist(MULTIREF_OFF),
 skip_id(UI_DEFAULT), match_id(UI_DEFAULT), match_prev(0),
 choice(0), chosen(false), choice_optional(false),
 sm(0)
 {
    moderror::err(err_msg);
 }


 moditem_list::~moditem_list()
 {
    #ifdef DEBUG
    std::cout << "~ui::moditem_list()" << std::endl;
    moditem* ui = goto_first();
    while(ui) {
        switch(ui->get_item_type()) {
        case ui::UI_ERROR: {
            std::cout << "UI_ERROR:\t\""
                      << ui->get_descr() << "\"";
            break;
        }
        case ui::UI_COMMENT: {
            modcomment* mc = static_cast<modcomment*>(ui);
            std::cout << "UI_COMMENT:\t\""
                      << mc->get_descr()
                      << "\"";
            break;
        }
        case ui::UI_PARAM: {
            modparam* mp = static_cast<modparam*>(ui);
            std::cout << "UI_PARAM:\t"
                      << param::names::get(mp->get_param_type());
            break;
        }
        case ui::UI_INPUT: {
            modinput* mi = static_cast<modinput*>(ui);
            std::cout << "UI_INPUT:\t"
                      << input::names::get(mi->get_input_type());
            break;
        }
        case ui::UI_DOBJ: {
            moddobj* md = static_cast<moddobj*>(ui);
            std::cout << "UI_DOBJ:\t"
                      << dobj::names::get(md->get_dobj_parent())
                      << ",\t"
                      << dobj::names::get(md->get_dobj_child());
            break;
        }
        default:
            std::cout << "ERROR: Unknown moditem";
        }
        std::cout << std::endl;
        ui = goto_next();
    }
    #endif
 }

 moditem* moditem_list::add_item(input::TYPE it)
 {
    moditem* i = new modinput(it);
    if (i) {
        if (!add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 moditem* moditem_list::add_item(param::TYPE pt)
 {
    moditem* i = new modparam(pt);
    if (i) {
        if (!add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 moditem* moditem_list::add_item(param::TYPE pt, const char* fixstr)
 {
    if (!fixstr)
        return 0;

    moditem* i = new modparam(pt);
    if (i) {
        llitem * li = add_at_tail(i);
        if (li) {
            if (!wcnt::get_fxsparamlist()->add_param(fixstr, pt)) {
                delete unlink_item(li);
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

 moditem* moditem_list::add_item(dobj::TYPE parent, dobj::TYPE child)
 {
    moditem* i = new moddobj(parent, child);
    if (i) {
        if (!add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 moditem* moditem_list::add_item(const char* comment)
 {
    moditem* i = new modcomment(comment);
    if (i) {
        if (!add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }


 bool moditem_list::validate(synthmod::base* sm, param::TYPE pt,
                                                errors::TYPE et)
 {
    synthmod::TYPE smt = sm->get_module_type();
    moditem* ui = goto_first();

    while(ui) {
        if (*ui == ui::UI_PARAM) {
            modparam* mp = static_cast<modparam*>(ui);
            if (*mp == pt)
                return ui->validate(sm, et);
        }
        ui = goto_next();
    }

    std::cout << "FIXME: parameter validation failed. Attempt to validate "
              << "parameter " << param::names::get(pt) << ", for which "
              << "module  type " << synthmod::names::get(smt)
              << " does not have."
              << std::endl;
    return false;
 }


 moditem* moditem_list::match_begin(synthmod::base* mod)
 {
    sm = mod;
    match_prev = 0;
    skip_id = match_id = UI_DEFAULT;
    choice = 0;
    chosen = false;
    choice_optional = false;
    return goto_first();
 }

 moditem* moditem_list::match_item(const char* str)
 {
    if (!str)
        return 0;

    llitem* li = sneak_current();

    if (!li)
        return 0;

    moditem* item = li->get_data();
    bool namechecked = false;

    do {
        if (item->get_item_type() == UI_COMMENT) {
            std::cout << "skipping comment '"
                      << item->get_descr() << "'" << std::endl;
            continue;
        }

        item->dump();
        if (match_prev) {
            if (match_prev->get_set_id() != item->get_set_id())
            {
                std::cout << "resetting skip/match id " << std::endl;
                skip_id = match_id = UI_DEFAULT;
            }
        }

        moditem* prev = match_prev;
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
                    uimoderr("must %s", "make a choice.");
                    return moderror::err();
                }
                choice = 0;
            }
        }

        int ret = item->is_match(str, skip_id, match_id);

        switch(ret) {
          case ITEM_SKIPPED:
            if (!namechecked) {
                if (strcmp(str, sm->get_username()) != 0
                 && param::names::type(str) == param::ERR_TYPE
                 && dobj::names::type(str) == dobj::ERR_TYPE
                 && input::names::type(str) == input::ERR_TYPE)
                {
                    uimoderr("'%s' is not recognised as a parameter name, "
                             "data object name, nor input name.", str);
                    return moderror::err();
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
            uimoderr("unexpected '%s', expected '%s'.", str, item->get_name());
            return moderror::err();
          case ITEM_SET_EXPECTED:
            uimoderr("'%s' must be specified after '%s'.", item->get_name(),
                                                           prev->get_name());
            return moderror::err();
          case ITEM_SET_UNEXPECTED:
            uimoderr("'%s' should not be specified when '%s' was not.",
                                         item->get_name(), prev->get_name());
            return moderror::err();
          case ITEM_CHOICE_EXPECTED:
            uimoderr("'%s' must be specified after '%s' " //****
                     "(not as specified '%s').", item->get_name(),
                                                 prev->get_name(), str);
            return moderror::err();
          case ITEM_CHOICE_UNEXPECTED:
            uimoderr("'%s' should not be specified after '%s'.",
                                         item->get_name(), choice->get_name());
            return moderror::err();
          default:
            uimoderr("%s unhandled ui item match return code (%d)",
                                            errors::stock::major, ret);
            return moderror::err();
            break;
        }

        //  item was skipped... 
        if (setid != UI_DEFAULT)
            skip_id = setid;

        // if it is an input then we must create
        //  a connector to ensure it is properly turned off...
        if (item->get_item_type() == UI_INPUT) {
            modinput* mi = static_cast<modinput*>(item);
            wcnt::get_connectlist()
                        ->add_connector_off(sm, mi->get_input_type());
        }

    } while((item = goto_next()) != 0);

    std::cout << "no more items.." << std::endl;
    return 0;
 }




}; //namespace ui
