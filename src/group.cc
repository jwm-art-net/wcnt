#include "../include/group.h"
#include "../include/globals.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/connectorlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"

#include <iostream>


group::group() :
 dobj::base(dobj::DEF_GROUP),
 is_duplicate(false), controlled(false)
{
}

group::group(CLONE) :
 dobj::base(dobj::DEF_GROUP),
 is_duplicate(true), controlled(false)
{
}

void group::register_ui()
{
    register_dobj(dobj::LST_MODULES, dobj::DOBJ_SYNTHMOD);
}

group::~group()
{
}

synthmod::base* group::group_module(synthmod::base* sm)
{
    const char* const grpname =
        set_groupname(get_username(), sm->get_username());
    if (wcnt::jwm.get_modlist()->get_synthmod_by_name(grpname))
    {
        dobjerr("Could not add %s to group %s. A module already exists "
                "named %s.", sm->get_username(), get_username(), grpname);
        delete [] grpname;
        invalidate();
        return 0;
    }
    delete [] grpname;
    if (!is_duplicate) {
        if (!groupify(sm)) {
            dobjerr("Could not add %s to group %s. It is forbidden to add "
                                                "a %s module to a group.",
                     sm->get_username(), get_username(),
                     synthmod::names::get(sm->get_module_type()));
            invalidate();
            return 0;
        }
    }
    // else { the module has already been groupified (it was duplicated
    // by the group that called this's private constructor.}
    if (wcnt::jwm.is_verbose()) {
        if (is_duplicate)
            std::cout << "\ngrouped synthmod ";
        else std::cout << "\nto ";
        std::cout << sm->get_username();
    }
    return sm;
}

dobj::base* group::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        const char* const grpname = sm->get_group_name();
        if (grpname) {
            dobjerr("Cannot add module to group %s, it is already in "
                    "the group %s.", get_username(), grpname);
            delete grpname;
            return 0;
        }
        if (!group_module((synthmod::base*)sm)) {
            /* err msgs generated in add_module (above) */
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
       wcnt::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }

    dobjerr("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
    return 0;
}

dobj::base* group::duplicate_dobj(const char* new_group_name)
{
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
            <<" to " << new_group_name << "... ";


    synthmod::list::linkedlist* grplist =
        new_list_of_by(wcnt::jwm.get_modlist(), groupname(get_username()));

    synthmod::base* mod = grplist->goto_first();
    synthmod::base* to_mod = duplist->goto_first();

    while(mod) {
        if (wcnt::jwm.is_verbose())
            std::cout << "\nchecking potential connections for " <<
                                                to_mod->get_username();
        connectorlist::linkedlist* conlist =
           wcnt::get_connectlist() ->
                duplicate_connections_for_module(mod, to_mod);
        connector* con = conlist->goto_first();
        while(con) {
            const char* const mod_groupname =
                get_groupname(con->get_output_module_name());
            if (mod_groupname) {
                if (strcmp(get_username(), mod_groupname) == 0) {
                    if (wcnt::jwm.is_verbose())
                        std::cout << "\nreforming connection " <<
                            con->get_output_module_name() << " to ";
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
                            << input::names::get(con->get_input_type());
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
}

bool group::groupify(synthmod::base* sm)
{
    if (sm->flag(synthmod::base::SM_UNGROUPABLE))
        return false;
    char* from = new char [strlen(sm->get_username()) + 1];
    strcpy(from, sm->get_username());
    sm->set_group_name(get_username());
   wcnt::get_connectlist()->reconnect_output_module_by_name(from,
                                                    sm->get_username());
    delete [] from;
    return true;
}

errors::TYPE group::validate()
{
    return errors::NO_ERROR;
}
