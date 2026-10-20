#include "../include/chain.h"
#include "../include/globals.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/connectorlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/group.h"

#include <iostream>


typedef groupnames gnames;


chain::chain() :
 dobj::base(dobj::DEF_CHAIN),
 copy_only(OFF),
 modules(0), connections(0), tmpdbjlist(0)
{
    set_flags(DO_ALLOW_IN_AUTOGROUP);
    modules = new synthmod::list(DELETE_DATA);
    connections = new connectorlist(DELETE_DATA);
}


void chain::register_ui()
{
    register_param(param::COPY_ONLY)->set_flags(ui::UI_OPTIONAL);
    register_dobj(dobj::LST_MODULES, dobj::DOBJ_SYNTHMOD);
}

ui::dobjitem_list* chain::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

chain::~chain()
{
    delete modules;
    delete connections;
    delete tmpdbjlist;
}

bool chain::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::COPY_ONLY:
            copy_only = (*(STATUS*)data);
            return true;
        default:
            return false;
    }
}

const void* chain::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::COPY_ONLY:  return &copy_only;
        default: return 0;
    }
}


synthmod::base* chain::chain_module(synthmod::base* sm)
{
    // this is used when the chain data object is defined
    // this function is called for every module listed
    // 1) duplicate module
    // 2) form chain name for module and rename module
    if (sm->flag(synthmod::base::SM_UNGROUPABLE)) {
        dobjerr("Module %s is forbidden from chain %s.", sm->get_username(),
                                                         get_username());
        return 0;
    }

    const char* name = sm->get_name(gnames::MODULE);
    char* chname = 0;

    if (copy_only == OFF) {
        chname = sm->make_username(gnames::CHAIN, get_username());

        if (!wcnt::jwm.get_modlist()->get_autogroup()) {
            char* tmp = chname;
            chname = gnames::str_make_username(tmp, gnames::GROUP, 0);
            std::cout << chname << std::endl;
            delete [] tmp;
        }

        if (wcnt::jwm.get_modlist()->get_synthmod_by_name(chname)) {
            dobjerr("Could not chain module %s as %s. A module already exists "
                    "named %s.", sm->get_username(), get_username(), chname);
            invalidate();
            delete [] chname;
            return 0;
        }
    }

    synthmod::base* nsm = sm->duplicate_module(name, synthmod::base::NO_CONNECT);
    if (!nsm) {
        dobjerr("Chain failed copying module %s.", name);
        invalidate();
        return 0;
    }

    modules->add_module(nsm);

    if (copy_only == OFF) {
        char* from = new char [strlen(sm->get_username()) + 1];
        strcpy(from, sm->get_username());
        // safe to set module username now
        sm->set_username(chname);

        if (wcnt::jwm.is_verbose()) {
            std::cout << "chaining module " << from << " --> ";
            std::cout << sm->get_username() << std::endl;
        }

        delete [] chname;
        chname = 0;

        if (wcnt::jwm.get_modlist()->get_autogroup()) {
            // connections during autogroup don't reflect the autogroup naming
            // until the end of the autogroup when they're updated...
            char* from_ng = gnames::str_make_username(from, gnames::GROUP, 0);
            char* to_ng = sm->make_username(gnames::GROUP, 0);
            wcnt::get_connectlist()->
                            reconnect_output_module_by_name(from_ng, to_ng);
            delete [] from_ng;
            delete [] to_ng;
        }
        else
            wcnt::get_connectlist()->
                    reconnect_output_module_by_name(from, sm->get_username());
        delete [] from;
    }

    if (!wcnt::get_connectlist()->
            duplicate_connections_to_connectorlist(sm, nsm, connections))
    {
        dobjerr("Could not chain module %s as %s. Connections failed.",
                sm->get_username(), get_username());
        invalidate();
        return 0;
    }

    return sm;
}


synthmod::base* chain::autochain_module(synthmod::base* sm)
{
    if (sm->flag(synthmod::base::SM_UNGROUPABLE))
        return 0;

    if (!sm->set_name(gnames::CHAIN, get_username()))
        return 0;

    return chain_module(sm);
}


bool chain::chainify(synthmod::base* sm)
{/*
    if (sm->flag(synthmod::base::SM_UNGROUPABLE))
        return false;
    char* from = new char [strlen(sm->get_username()) + 1];
    strcpy(from, sm->get_username());
    //sm->set_group_name(get_username());
    wcnt::get_connectlist()->reconnect_output_module_by_name(from, sm->get_username());
    delete [] from;
    */
    return true;
}


dobj::base* chain::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        if (!tmpdbjlist && (!(tmpdbjlist = new dobj::list)))
            return 0;
        return tmpdbjlist->add_dobj(dbj);
    }

    dobjerr("%s %s to %s", errors::stock::major,
            errors::stock::bad_add, get_username());
    return 0;
}


bool chain::do_actions()
{
    if (!tmpdbjlist)
        return true;

    dobj::base* dbj = tmpdbjlist->goto_first();

    while (dbj) {
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!chain_module((synthmod::base*)sm))
            return false;
        dbj = tmpdbjlist->goto_next();
    }
    delete tmpdbjlist;
    tmpdbjlist = 0;
    return true;
}


dobj::base* chain::duplicate_dobj(const char* new_chain_name)
{
    // the chain doesn't duplicate itself, it only duplicates the modules
    // and their connections managed by it's own private data.

    if (wcnt::jwm.is_verbose()) {
        std::cout << "\nDuplicating modules in chain " << get_username();
        std::cout << " to " << new_chain_name << "... " << std::endl;
    }

    group* ag = wcnt::jwm.get_modlist()->get_autogroup();
    const char* agn = (ag ? ag->get_username() : 0);

    synthmod::base* sm = modules->goto_first();
    while(sm) {
        char* cn = sm->make_username(gnames::CHAIN, new_chain_name);
        synthmod::base* sm2 = sm->duplicate_module(cn, synthmod::base::NO_CONNECT);
        sm2->set_name(gnames::GROUP, agn);
        wcnt::jwm.get_modlist()->add_module(sm2);
        if (wcnt::jwm.is_verbose()) {
            std::cout << "Duplicating module " << sm->get_username()
                << " --> " << sm2->get_username() << std::endl;
        }
        // reform connections for the new module...
        connectorlist::linkedlist* conlist =
                        connections->duplicate_connections_for_module(sm, sm2);
        connector* con = conlist->goto_first();
        if (con && wcnt::jwm.is_verbose())
            std::cout << "Reforming connections..." << std::endl;
        while(con) {
            // don't forget module name in connectors internal to chain data object
            // was stripped of group/chain components...
            char* chn = gnames::str_make_username(con->get_output_module_name(),
                                                gnames::CHAIN, new_chain_name);
            // this might be inside an autogroup...
            char* omn = gnames::str_make_username(chn, gnames::GROUP, agn);
            delete [] chn;
            con->set_output_module_name(omn);
            if (wcnt::jwm.is_verbose()) {
                std::cout << omn << " "
                << output::names::get(con->get_output_type())
                << " --> "
                << con->get_input_module()->get_username()
                << " "
                << input::names::get(con->get_input_type()) << std::endl;
            }
            delete [] omn;

            wcnt::get_connectlist()->add_connector(con);
            con = conlist->goto_next();
        }
        delete [] cn;
        delete conlist;
        sm = modules->goto_next();
    }
    return this;
}

errors::TYPE chain::validate()
{
    return errors::NO_ERROR;
}


