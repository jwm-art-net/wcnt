#include "../include/ui_moditem.h"
#include "../include/checkvalue.h"
#include "../include/fxsparamlist.h"
#include "../include/globals.h"
#include "../include/listwork.h"

#include <iostream>


namespace ui
{

 moditem_list::moditem_list() :
 linkedlist(MULTIREF_OFF),
 skip_id(UI_DEFAULT), match_id(UI_DEFAULT)
 {
 }

 moditem_list::moditem_list(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d),
 skip_id(UI_DEFAULT), match_id(UI_DEFAULT)
 {
 }
 moditem_list::~moditem_list()
 {
    #ifdef DEBUG
    std::cout << "~ui::moditem_list()" << std::endl;
    moditem* ui = goto_first();
    while(ui) {
        switch(ui->get_item_type()) {
        case ui::UI_COMMENT: {
            modcomment* mc = static_cast<modcomment*>(ui);
            std::cout << "UI_COMMENT:\t\""
                      << mc->get_comment()
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
            std::cout << "UI_ERROR";
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

}; //namespace ui
