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
    D_CONNECT("chain_module called with name %s\n", sm->get_username());

    if (sm->flag(synthmod::base::SM_UNGROUPABLE)) {
        dobjerr("Module %s is forbidden from chain %s.", sm->get_username(),
                                                         get_username());
        return 0;
    }

    const char* name = sm->get_name(gnames::MODULE);
    char* chname = 0;

    // prove me wrong, but won't (ac == this) == true
    chain* ac = wcnt::jwm.get_modlist()->get_autochain();

    if (copy_only == OFF) {
        chname = sm->make_username(gnames::CHAIN, get_username());

        D_CONNECT("calling get_synthmod_by_name with %s\n", chname);

        if (!wcnt::jwm.get_modlist()->get_autogroup()) {
            char* tmp = chname;
            chname = gnames::str_make_username(tmp, gnames::GROUP, 0);
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
    else
        chname = gnames::str_make_username(sm->get_username());

    synthmod::base* nsm;

    if (ac && ac->is_copy_only()) {
        // autochain_copy_only - don't duplicate it, just add it
        nsm = sm;
    }
    else {
        nsm = sm->duplicate_module(name, synthmod::base::NO_CONNECT);
        if (!nsm) {
            dobjerr("Chain failed copying module %s.", name);
            invalidate();
            return 0;
        }
    }

    modules->add_module(nsm);

    if (copy_only == OFF) {
        char* from = gnames::str_make_username(sm->get_username());
        // safe to rename original module now
        sm->set_username(chname);

        if (wcnt::jwm.is_verbose()) {
            std::cout << "chaining module " << from << " --> ";
            std::cout << sm->get_username() << std::endl;
        }

        D_CONNECT("reconnecting ouput module by name...\n");

        // reconnect using new name for original module
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
                    reconnect_output_module_by_name(from, chname);
        delete [] from;
    }

    D_CONNECT("duplicating connections (from %s to %s) "
    "to internal list...\n",
    sm->get_username(), nsm->get_username());

    connectorlist* conlist = wcnt::get_connectlist();
    connector* con = conlist->goto_first();

    while(con) {
        con->dump_info("comparing connector: ");
        if (strcmp(con->get_input_module()->get_username(), chname) == 0) {
            con->dump_info("duplicating connector: ");
            connector* nc = con->duplicate(nsm);
            const char* omn = nc->get_output_module_name();
            char* ocn = gnames::str_get_name(omn, gnames::CHAIN);

            if (ocn && strcmp(get_username(), ocn) == 0) {
                // output module chain matches, remove
                char* tmp = gnames::str_make_username(omn, gnames::CHAIN, 0);
                nc->set_output_module_name(tmp);
                delete [] tmp;
            }
            delete [] ocn;
            nc->dump_info();
            connections->add_at_tail(nc);
        }
        if (ac && ac->is_copy_only()) {
            connector* tmp = con;
            con = conlist->goto_next();
            conlist->delete_connector(tmp);
        }
        else
            con = conlist->goto_next();
    }

    delete [] chname;

    return sm;
}


synthmod::base* chain::autochain_module(synthmod::base* sm)
{
    D_CONNECT("autochain module %s\n", sm->get_username());
    if (sm->flag(synthmod::base::SM_UNGROUPABLE))
        return 0;

    if (copy_only == OFF) {
        if (!sm->set_name(gnames::CHAIN, get_username()))
            return 0;
    }

    return chain_module(sm);
}


dobj::base* chain::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        #ifdef DEBUG_CONNECT
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        D_CONNECT("add dobj (module) %s\n", sm->get_username());
        #endif
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
    chain* ac = wcnt::jwm.get_modlist()->get_autochain();

    synthmod::base* sm = modules->goto_first();
    while(sm) {
        D_CONNECT("maybe duplicating chain module %s\n", sm->get_username());
        char* cn = sm->make_username(gnames::CHAIN, new_chain_name);
        if (ac && ac != this) {
            // autochain active, and this chain is being duplicated inside it.
            cn = gnames::str_flatten_chain_to_module_name(cn);
            D_CONNECT("chain name %s flattened to module name %s.\n",
                                                            new_chain_name, cn);
        }
        synthmod::base* sm2 = sm->duplicate_module(cn, synthmod::base::NO_CONNECT);

        if (ac && ac != this && ac->is_copy_only()) {
            D_CONNECT("not adding %s to global userspace\n", sm2->get_username());
            // need to sort connections before chaining it in the autogroup
        }
        else {
            wcnt::jwm.get_modlist()->add_module(sm2);
            sm2->set_name(gnames::GROUP, agn);
        }
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
            con->dump_info("updating connector: ");
            if (gnames::str_has_name(con->get_output_module_name(), gnames::CHAIN)) {
                D_CONNECT("connector output module has chained name\n");
            }
            D_CONNECT("-- output module name: %s\n", con->get_output_module_name());
            char* omn = gnames::str_make_username(con->get_output_module_name());
            if (modules->get_synthmod_by_name(omn)) {
                // output module is part of the chain, name it so
                char* tmp = omn;
                omn = gnames::str_make_username(omn, gnames::CHAIN,
                                                     new_chain_name);
                delete [] tmp;
                if (ac && ac != this) { // autochain active, and this cahin is
                                        // being duplicatedwithin it
                    omn = gnames::str_flatten_chain_to_module_name(omn);
                    char* tmp = omn;
                    omn = gnames::str_make_username(omn, gnames::CHAIN,
                                                         ac->get_username());
                    delete [] tmp;
                }
                if (agn) {
                    // and we're in an autogroup
                    tmp = omn;
                    omn = gnames::str_make_username(omn, gnames::GROUP, agn);
                    delete [] tmp;
                }
            }
            D_CONNECT("-- setting output module name: %s\n", omn);
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

            // add connector to global user space in all circumstances
            // even ac && ac != this && ac->is_copy_only
            wcnt::get_connectlist()->add_connector(con);
            con = conlist->goto_next();
        }
        delete [] cn;
        delete conlist;
        if (ac && ac != this)
            ac->chain_module(sm2);
        sm = modules->goto_next();
    }
    return this;
}

errors::TYPE chain::validate()
{
    return errors::NO_ERROR;
}


