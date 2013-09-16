#include "../include/group.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/connectorlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/topdobjlist.h"
#include "../include/dobjmod.h"

#include <iostream>

group::group() :
 dobj(dobjnames::DEF_GROUP),
 is_duplicate(false), controlled(false)
{
    create_params();
}

group::group(CLONE) :
 dobj(dobjnames::DEF_GROUP),
 is_duplicate(true), controlled(false)
{
    create_params();
}

group::~group()
{
}

synthmod* group::group_module(synthmod* sm)
{
    const char* const grpname =
        set_groupname(get_username(), sm->get_username());
    if (jwm.get_modlist()->get_synthmod_by_name(grpname))
    {
        *err_msg = "\ncould not add ";
        *err_msg += sm->get_username();
        *err_msg += " to group ";
        *err_msg += get_username();
        *err_msg += ". A module already exists called ";
        *err_msg += grpname;
        delete [] grpname;
        invalidate();
        return 0;
    }
    delete [] grpname;
    if (!is_duplicate) {
        if (!groupify(sm)) {
            *err_msg = "\ncould not add ";
            *err_msg += sm->get_username();
            *err_msg += " to group ";
            *err_msg += get_username();
            *err_msg += ".\nit is forbidden to add a ";
            *err_msg +=
                jwm.get_modnames()->get_name(sm->get_module_type());
            *err_msg += " module to a group.";
            invalidate();
            return 0;
        }
    }
    // else { the module has already been groupified (it was duplicated
    // by the group that called this's private constructor.}
    if (jwm.is_verbose()) {
        if (is_duplicate)
            std::cout << "\ngrouped synthmod ";
        else std::cout << "\nto ";
        std::cout << sm->get_username();
    }
    return sm;
}

dobj* group::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        const char* const grpname = sm->get_group_name();
        if (grpname) {
            *err_msg = "\ncannot add module to group ";
            *err_msg += get_username();
            *err_msg += ". it is already part of the ";
            *err_msg += grpname;
            *err_msg += " group.";
            delete grpname;
            return 0;
        }
        if (!group_module((synthmod*)sm)) {
            /* err msgs generated in add_module (above) */
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += *get_username();
    return 0;
}

dobj* group::duplicate_dobj(const char* new_group_name)
{
    if (jwm.is_verbose()) {
        std::cout << "\nDuplicating modules in group " << get_username();
        std::cout << " to " << new_group_name << "... ";
    }

    synthmodlist::linkedlist* duplist =
        jwm.get_modlist()->
            duplicate_group(get_username(), new_group_name);

    if (!duplist)
    {
        *err_msg = "\nduplication of modules within group ";
        *err_msg += *get_username();
        *err_msg += " failed.";
        return 0;
    }

    if (jwm.is_verbose()) {
        std::cout << "Ok";
    }

    group* dup = new group(DUPLICATE);
    dup->set_username(new_group_name);

    // now reconnect the duplicated modules.
    if (jwm.is_verbose())
        std::cout
            << "\nDuplicating connections in group " << get_username()
            <<" to " << new_group_name << "... ";


    synthmodlist::linkedlist* grplist =
        new_list_of_by(jwm.get_modlist(), groupname(get_username()));

    synthmod* mod = grplist->goto_first();
    synthmod* to_mod = duplist->goto_first();

    while(mod) {
        if (jwm.is_verbose())
            std::cout << "\nchecking potential connections for " <<
                                                to_mod->get_username();
        connectorlist::linkedlist* conlist =
            jwm.get_connectlist() ->
                duplicate_connections_for_module(mod, to_mod);
        connector* con = conlist->goto_first();
        while(con) {
            const char* const mod_groupname =
                get_groupname(con->get_output_module_name());
            if (mod_groupname) {
                if (strcmp(get_username(), mod_groupname) == 0) {
                    if (jwm.is_verbose())
                        std::cout << "\nreforming connection " <<
                            con->get_output_module_name() << " to ";
                    const char* const new_mod_name =
                        set_groupname(new_group_name,
                            con->get_output_module_name());
                    con->set_output_module_name(new_mod_name);
                    if (jwm.is_verbose()) {
                        std::cout << new_mod_name << " "
                            << jwm.get_outputnames()
                              ->  get_name(con->get_output_type())
                            << " --> "
                            << con->get_input_module()->get_username()
                            << " " << jwm.get_inputnames()
                              ->  get_name(con->get_input_type());
                    }
                    delete [] new_mod_name;
                }
                delete [] mod_groupname;
            }
            jwm.get_connectlist()->add_connector(con);
            con = conlist->goto_next();
        }
        delete conlist;
        mod = grplist->goto_next();
        to_mod = duplist->goto_next();
    }
    if (jwm.is_verbose())
        std::cout << std::endl;
    delete duplist;
    delete grplist;
    return dup;
}

bool group::groupify(synthmod* sm)
{
    if (sm->flag(synthmod::SM_UNGROUPABLE))
        return false;
    char* from = new char [strlen(sm->get_username()) + 1];
    strcpy(from, sm->get_username());
    sm->set_group_name(get_username());
    jwm.get_connectlist()->reconnect_output_module_by_name(from,
                                                    sm->get_username());
    delete [] from;
    return true;
}

stockerrs::ERR_TYPE group::validate()
{
    return stockerrs::ERR_NO_ERROR;
}

void group::create_params()
{
    if (done_params())
        return;
    jwm.get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_GROUP, dobjnames::LST_MODULES)
            ->add_dobjdobj(dobjnames::LST_MODULES,
                dobjnames::DOBJ_SYNTHMOD);
}


