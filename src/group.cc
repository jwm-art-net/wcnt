#ifndef GROUP_H
#include "../include/group.h"
#include "../include/jwm_globals.h"
#include "../include/synthmodule.h"
#include "../include/synthmodulelist.h"
#include "../include/connectorlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/topdobjlist.h"
#include "../include/dobjmod.h"

#include <iostream>

group::group() :
 dobj(dobjnames::DEF_GROUP), is_duplicate(false),
 modlist(0), mod_item(0), mod(0)

{
    modlist =
        new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    create_params();
}

group::group(CLONE) :
 dobj(dobjnames::DEF_GROUP), is_duplicate(true),
 modlist(0), mod_item(0), mod(0)

{
    modlist =
        new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    create_params();
}

group::~group()
{
    delete modlist; // only delete the list, not the modules. ok.
}

synthmod* group::add_module(synthmod* sm)
{
    const char* const grpname =
        set_groupname(get_username(), sm->get_username());
    if (jwm.get_modlist().get_synthmod_by_name(grpname))
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
    if (!(mod_item = modlist->add_at_tail(sm))) {
        *err_msg = "\ncould not add ";
        *err_msg += sm->get_username();
        *err_msg += " to group ";
        *err_msg += get_username();
        *err_msg += ". maybe you've already added it?";
        invalidate();
        return 0;
    }
    if (!is_duplicate) {
        if (!groupify(sm)) {
            *err_msg = "\ncould not add ";
            *err_msg += sm->get_username();
            *err_msg += " to group ";
            *err_msg += get_username();
            *err_msg += ". it wants to be left alone.";
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
    return (synthmod*)mod_item->get_data();
}

dobj* group::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        const char* const grpname = sm->get_group_name();
        if (grpname) {
            *err_msg = "\ncannot add module to group ";
            *err_msg += get_username();
            *err_msg += ". it is already in the ";
            *err_msg += grpname;
            *err_msg += " group.";
            delete grpname;
            return 0;
        }
        if (!add_module((synthmod*)sm)) {
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist().add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += *get_username();
    return 0;
}

dobj* group::duplicate_dobj(const char* new_group_name)
{
    synthmod* to_mod;
    group* dup = new group(DUPLICATE);
    dup->set_username(new_group_name);
    // now the group is duplicated, lets duplicate the groups modules.
    goto_first();
    if (jwm.is_verbose()) {
        std::cout << "\nDuplicating group " << get_username();
        std::cout << " to " << new_group_name;
    }
    while (mod) {
        const char* const to_name =
                    set_groupname(new_group_name, mod->get_username());
        to_mod = mod->duplicate_module(to_name, synthmod::NO_CONNECT);
        delete [] to_name;
        if (!to_mod) {
            *err_msg = "\nmodule type ";
            *err_msg += jwm.get_modnames().get_name(
                                                mod->get_module_type());
            *err_msg += *synthmod::get_error_msg();
            *err_msg += " could not be duplicated to be added to group.";
            delete dup;
            return 0;
        }
        if  (!dup->add_module(to_mod)) {
            delete dup;
            return 0;
        }
        jwm.get_modlist().add_module(to_mod);
        goto_next();
    }
    // now reconnect the duplicated modules.
    if (jwm.is_verbose())
        std::cout
            << "\nDuplicating group " << get_username() <<" connections";
    goto_first();
    to_mod = dup->goto_first();
    connectorlist& smconlist = jwm.get_connectlist();
    while(mod) {
        connectorlist* conlist =
            smconlist.duplicate_connections_for_module(mod, to_mod);
        connector* con = conlist->goto_first();
        if (jwm.is_verbose())
            std::cout << "\nchecking potential connections for " <<
                                                to_mod->get_username();
        while(con) {
            const char* const old_outmodname =
                con->get_output_module_name();
            if (strstr(old_outmodname, ".")) {
                const char* const old_groupname = get_username();
                char* cur_grpname = new char[strlen(old_outmodname) + 1];
                strcpy(cur_grpname, old_outmodname);
                char* dot_ptr = strstr(cur_grpname, ".");
                *dot_ptr = '\0';
                if (strcmp(cur_grpname, old_groupname) == 0) {
                    if (jwm.is_verbose())
                        std::cout << "\nreforming connection " <<
                                                old_outmodname << " to ";
                    const char* const new_outmodname =
                            set_groupname(new_group_name, old_outmodname);
                    con->set_output_module_name(new_outmodname);
                    if (jwm.is_verbose()) {
                        std::cout << new_outmodname << " " <<
                            jwm.get_outputnames().
                                        get_name(con->get_output_type());
                        std::cout << " --> " <<
                                con->get_input_module()->get_username();
                        std::cout << " " << jwm.get_inputnames().
                                        get_name(con->get_input_type());
                    }
                    delete [] new_outmodname;
                }
                *dot_ptr = '.';
                delete [] cur_grpname;
            }
            smconlist.add_connector(con);
            con = conlist->goto_next();
        }
        delete conlist;
        goto_next();
        to_mod = dup->goto_next();
    }
    if (jwm.is_verbose())
        std::cout << std::endl;
    return dup;
}

bool group::groupify(synthmod* sm)
{
    char* from = new char [strlen(sm->get_username()) + 1];
    strcpy(from, sm->get_username());
    sm->set_group_name(get_username());
    jwm.get_connectlist().reconnect_output_module_by_name(from,
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
    if (group::done_params == true)
        return;
    dobjdobjlist* dbjlist = jwm.get_topdobjlist().create_dobjdobjlist(
                            dobjnames::DEF_GROUP, dobjnames::LST_MODULES);
    if (!dbjlist->add_dobjdobj(
        dobjnames::LST_MODULES, dobjnames::DOBJ_SYNTHMOD))
    {
        std::cout << "\ncould not create group::stuff;";
    }
    done_params = true;
}

bool group::done_params = false;


#endif
