#ifndef DOBJLIST_H
#include "../include/dobjlist.h"

dobjlist::dobjlist() :
 dobj_list(0), dobj_item(0), d_obj(0),
 search_dobj_type(dobjnames::DOBJ_FIRST), dobj_search(0)
{
    dobj_list = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

dobjlist::~dobjlist()
{
    goto_first();
    while(d_obj) {
        delete d_obj;
        goto_next();
    }
    delete dobj_list;
}

dobj* dobjlist::add_dobj(dobj* d)
{
    if (!(dobj_item = dobj_list->add_at_tail(d))) return 0;
    return (d_obj = (dobj*)dobj_item->get_data());
}

dobj* dobjlist::create_dobj(dobjnames::DOBJ_TYPE dt)
{
    if (dobj::get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
        return 0;
    dobj* dobjy = 0;
    switch(dt) {
    case dobjnames::SIN_NOTE:
        dobjy = new note_data;
        break;
    case dobjnames::SIN_COORD:
        dobjy = new adsr_coord;
        break;
    case dobjnames::SIN_VERTEX:
        dobjy = new wave_vertex;
        break;
    case dobjnames::SIN_METER:
        dobjy = new meterchange;
        break;
    case dobjnames::SIN_BPM:
        dobjy = new bpmchange;
        break;
    case dobjnames::SIN_RIFFNODE:
        dobjy = new riff_node;
        break;
    case dobjnames::SIN_DVERTEX:
        dobjy = new dynvertex;
        break;
    case dobjnames::SIN_TIME:
        dobjy = new timing;
        break;
    case dobjnames::SIN_MODNAME:
        dobjy = new modnamedobj;
        break;
    case dobjnames::SIN_DOBJNAME:
        dobjy = new dobjnamedobj;
        break;
    case dobjnames::SIN_EDIT_PARAM:
        dobjy = new paramedit;
        break;
    case dobjnames::SIN_EDIT_INPUT:
        dobjy = new inputedit;
        break;
    case dobjnames::DEF_WAVFILEIN:
        dobjy = new wavfilein;
        break;
    case dobjnames::DEF_RIFF:
        dobjy = new riffdata;
        break;
    case dobjnames::DEF_WCFILE:
        dobjy = new synthfilereader;
        break;
    case dobjnames::DEF_PARAMEDITOR:
        dobjy = new parameditor;
        break;
    case dobjnames::DEF_INPUTEDITOR:
        dobjy = new inputeditor;
        break;
    case dobjnames::DOBJ_SYNTHMOD:
        dobjy = new dobjmod;
        break;
    default:
        dobjy = 0;
    }
    return dobjy;
}

dobj* dobjlist::get_dobj_by_name(char const* dname)
{
    goto_first();
    while(d_obj) {
        // not all dobjs in list are user defined, some are
        // added by modules that use lists of wcnt_signal
        // modules, and thus don't have a username.
        if (d_obj->get_username()) {
            if (strcmp(d_obj->get_username(), dname) == 0)
                return d_obj;
        }
        goto_next();
    }
    return 0;
}

dobj* dobjlist::get_first_of_dobj_type(dobjnames::DOBJ_TYPE dt)
{
    dobj_search = 0;
    search_dobj_type = dobjnames::DOBJ_FIRST;
    if (dobj::get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
        return 0;
    goto_first();
    while(d_obj) {
        if (d_obj->get_object_type() == dt) {
            dobj_search = dobj_item;
            search_dobj_type = dt;
            return d_obj;
        }
        goto_next();
    }
    return 0;
}

dobj* dobjlist::get_next_of_dobj_type()
{
    if (dobj::get_dobjnames()->check_type(search_dobj_type)
            == dobjnames::DOBJ_FIRST)
        return 0;
    dobj_item = dobj_list->goto_item(dobj_search);
    goto_next();
    while(d_obj) {
        if (d_obj->get_object_type() == search_dobj_type) {
            dobj_search = dobj_item;
            return d_obj;
        }
        goto_next();
    }
    dobj_search = 0;
    search_dobj_type = dobjnames::DOBJ_FIRST;
    return 0;
}

#endif
