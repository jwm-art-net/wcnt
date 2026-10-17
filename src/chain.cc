#include "../include/chain.h"
#include "../include/globals.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/connectorlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"

#include <iostream>


chain::chain() :
 dobj::base(dobj::DEF_CHAIN),
 modules(0), connections(0)
{
    modules = new synthmod::list(DELETE_DATA);
    connections = new connectorlist(DELETE_DATA);
}


void chain::register_ui()
{
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
}

synthmod::base* chain::chain_module(synthmod::base* sm)
{
    // this is used when the chain data object is defined
    // this function is called for every module listed
    // 1) duplicate module
    // 2) form chain name for module and rename module
    std::cout << "\\--> chaining module... " << sm->get_username() << std::endl;


    // FIXME: chained module name needs to be updated, currently there is no
    // method for changing a synthmod name


    if (sm->flag(synthmod::base::SM_UNGROUPABLE)) {
        dobjerr("Module %s is forbidden from chain %s.", sm->get_username(), get_username());
        return 0;
    }

    // name of module sans groupname
    const char* name = sm->get_username();
    const char* p = strchr(name, '.');
    if (p)
        name = ++p;

    if (*p == '\0') {
        dobjerr("Module %s has invalid name.", name);
        invalidate();
        return 0;
    }

    std::cout << "\\--> internal module name... " << name << std::endl;

    const char* chname = sm->set_name(groupnames::CHAIN, get_username());

    if (wcnt::jwm.get_modlist()->get_synthmod_by_name(chname))
    {
        dobjerr("Could not chain module %s as %s. A module already exists "
        "named %s.", sm->get_username(), get_username(), chname);
        invalidate();
        return 0;
    }

    synthmod::base* nsm = sm->duplicate_module(chname, synthmod::base::NO_CONNECT);
    if (!nsm) {
        dobjerr("Chain failed copying module %s.", name);
        invalidate();
        return 0;
    }

    modules->add_module(nsm);

    if (!wcnt::get_connectlist()->duplicate_connections_to_connectorlist(sm, nsm, connections)) {
        dobjerr("Could not chain module %s as %s. Connections failed.",
                sm->get_username(), get_username());
        invalidate();
        return 0;
    }

    return sm;
}


synthmod::base* chain::auto_chain_module(synthmod::base* sm)
{
    // FIXME: determine if using SM_UNGROUPABLE is adequate or if a
    // specialized SM_UNCHAINABLE is required
    if (sm->flag(synthmod::base::SM_UNGROUPABLE))
        return 0;

//    sm->set_group_name(get_username());
    return sm;
}


dobj::base* chain::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!chain_module((synthmod::base*)sm)) {
            return 0;
        }
        // add dobj synthmod wrapper to global dobjlist
        // so it is deleted before program exit.
        wcnt::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }

    dobjerr("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
    return 0;
}

dobj::base* chain::duplicate_dobj(const char* new_chain_name)
{
    // we don't duplicate the chain here, we only duplicate the modules and
    // their connections the chain consists of, as chain is a bit like a copy
    // and paste mechanism for modules
/*
    if (wcnt::jwm.is_verbose()) {
        std::cout << "\nDuplicating modules in group " << get_username();
        std::cout << " to " << new_group_name << "... ";
    }

    synthmod::list::linkedlist* duplist =
        wcnt::jwm.get_modlist()->
            duplicate_group(get_username(), new_group_name);

    if (!duplist)
    {
        dobjerr("Duplication of modules within group %s failed.",
                                                get_username());
        return 0;
    }

    if (wcnt::jwm.is_verbose()) {
        std::cout << "Ok";
    }

    group* dup = new group(DUPLICATE);
    dup->set_username(new_group_name);

    // now reconnect the duplicated modules.
    if (wcnt::jwm.is_verbose())
        std::cout
            << "\nDuplicating connections in group " << get_username()
            <<" to " << new_group_name << "... " << std::endl;;


    synthmod::list::linkedlist* grplist =
        new_list_of_by(wcnt::jwm.get_modlist(), groupname(get_username()));

    synthmod::base* mod = grplist->goto_first();
    synthmod::base* to_mod = duplist->goto_first();

    while(mod) {
        D_CONNECT("Checking connections for module %s\n", mod->get_username());
        connectorlist::linkedlist* conlist =
           wcnt::get_connectlist() ->
                duplicate_connections_for_module(mod, to_mod);
        connector* con = conlist->goto_first();
        while(con) {
            D_CONNECT("-- output module name %s\n", con->get_output_module_name());
            const char* const mod_groupname =
                get_groupname(con->get_output_module_name());
            if (mod_groupname) {
                if (strcmp(get_username(), mod_groupname) == 0) {
                    if (wcnt::jwm.is_verbose())
                        std::cout << "Reforming connection "
                                  << con->get_output_module_name() << " to ";
                    const char* const new_mod_name =
                        set_groupname(new_group_name,
                            con->get_output_module_name());
                    con->set_output_module_name(new_mod_name);
                    if (wcnt::jwm.is_verbose()) {
                        std::cout << new_mod_name << " "
                            << output::names::get(con->get_output_type())
                            << " --> "
                            << con->get_input_module()->get_username()
                            << " "
                            << input::names::get(con->get_input_type()) << std::endl;;
                    }
                    delete [] new_mod_name;
                }
                delete [] mod_groupname;
            }
           wcnt::get_connectlist()->add_connector(con);
            con = conlist->goto_next();
        }
        delete conlist;
        mod = grplist->goto_next();
        to_mod = duplist->goto_next();
    }
    if (wcnt::jwm.is_verbose())
        std::cout << std::endl;
    delete duplist;
    delete grplist;
    return dup;
    */
return 0;
}

bool chain::chainify(synthmod::base* sm)
{
    // FIXME: still group code
    if (sm->flag(synthmod::base::SM_UNGROUPABLE))
        return false;
    char* from = new char [strlen(sm->get_username()) + 1];
    strcpy(from, sm->get_username());
    //sm->set_group_name(get_username());
    wcnt::get_connectlist()->reconnect_output_module_by_name(from,
                                                    sm->get_username());
    delete [] from;
    return true;
}

errors::TYPE chain::validate()
{
    return errors::NO_ERROR;
}


