#include "../include/ui_dobjitem.h"
#include "../include/checkvalue.h"
#include "../include/dobj.h"
#include "../include/fxsparamlist.h"
#include "../include/globals.h"
#include "../include/listwork.h"

#include <iostream>


namespace ui
{

 dobjitem_list::dobjitem_list() :
  search(dobj::ERR_TYPE), result(0)
 {
 }

 dobjitem_list::dobjitem_list(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
 {
 }
 dobjitem_list::~dobjitem_list()
 {
    #ifdef DEBUG
    std::cout << "~ui::dobjitem_list()" << std::endl;
    dobjitem* ui = goto_first();
    while(ui) {
        std::cout << "ui::dobjitem::"
                  << dobj::names::get(ui->get_data_type()) << "\t";
        switch(ui->get_item_type()) {
        case ui::UI_PARAM: {
            dobjparam* mp = static_cast<dobjparam*>(ui);
            std::cout << "UI_PARAM:\t"
                      << param::names::get(mp->get_param_type());
            break;
        }
        case ui::UI_DOBJ: {
            dobjdobj* md = static_cast<dobjdobj*>(ui);
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

 dobjitem* dobjitem_list::add_item(dobj::TYPE dt, param::TYPE pt)
 {
    dobjitem* i = new dobjparam(dt, pt);
    if (i) {
        if (!ordered_insert(this, i, &dobjitem::get_data_type)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }

 dobjitem* dobjitem_list::add_item(dobj::TYPE dt, param::TYPE pt,
                                                     const char* fixstr)
 {
    if (!fixstr)
        return 0;

    dobjitem* i = new dobjparam(dt, pt);
    if (i) {
        llitem * li = ordered_insert(this, i, &dobjitem::get_data_type);
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

 dobjitem* dobjitem_list::add_item(dobj::TYPE dt, dobj::TYPE parent,
                                                  dobj::TYPE child)
 {
    dobjitem* i = new dobjdobj(dt, parent, child);
    if (i) {
        if (!ordered_insert(this, i, &dobjitem::get_data_type)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }


 dobjitem* dobjitem_list::add_item(dobj::TYPE dt, const char* comment)
 {
    dobjitem* i = new dobjcomment(dt, comment);
    if (i) {
        if (!ordered_insert(this, i, &dobjitem::get_data_type)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }


 bool dobjitem_list::validate(dobj::base* dob, param::TYPE pt,
                                               errors::TYPE et)
 {
    dobj::TYPE dt = dob->get_object_type();
    dobjitem* ui = get_first_of_type(dt);

    while(result) {
        if (*ui == ui::UI_PARAM) {
            dobjparam* mp = static_cast<dobjparam*>(ui);
            if (*mp == pt)
                return ui->validate(dob, et);
        }
        ui = get_next_of_type();
    }

    std::cout << "FIXME: parameter validation failed. Attempt to validate "
              << "parameter " << param::names::get(pt) << ", for which "
              << "data object type " << dobj::names::get(dt)
              << " does not have."
              << std::endl;
    return false;
 }

}; //namespace ui
