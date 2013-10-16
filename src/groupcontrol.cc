#include "../include/groupcontrol.h"
#include "../include/dobjlist.h"
#include "../include/globals.h"
#include "../include/group.h"
#include "../include/synthmodlist.h"

#include <iostream>
#include <sstream>

group_control::group_control(const char* uname) :
 synthmod::base(synthmod::GROUPCONTROL, uname, SM_UNGROUPABLE | SM_UNDUPLICABLE),
 in_play_trig(0), in_stop_trig(0),
 out_play_state(OFF),
 group_name(0),
 stop_pending(0),
 grp(0), runlist(0), empty_run_list(0)
{
    register_output(output::OUT_PLAY_STATE);
}

void group_control::register_ui()
{
    register_input(input::IN_PLAY_TRIG);
    register_input(input::IN_STOP_TRIG);
    register_param(param::GROUP_NAME);
}

group_control::~group_control()
{
    if (group_name)
        delete [] group_name;
    if (runlist)
        destroy_array_moved_from_list(runlist);
    if (empty_run_list)
        destroy_array_moved_from_list(empty_run_list);
}

const void* group_control::get_out(output::TYPE ot) const
{
    switch (ot)
    {
        case output::OUT_PLAY_STATE: return &out_play_state;
        default: return 0;
    }
}

const void* group_control::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_PLAY_TRIG:
            return in_play_trig = (STATUS const*)o;
        case input::IN_STOP_TRIG:
            return in_stop_trig = (STATUS const*)o;
        default:
            return 0;
    }
}

const void* group_control::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_PLAY_TRIG: return in_play_trig;
        case input::IN_STOP_TRIG: return in_stop_trig;
        default: return 0;
    }
}

bool group_control::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::GROUP_NAME:
            return set_group_name((const char*)data);
        default:
            return false;
    }
}

const void* group_control::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::GROUP_NAME: return group_name;
        default: return 0;
    }
}

errors::TYPE group_control::validate()
{
    // validation done at set_param level.
    return errors::NO_ERROR;
}

void group_control::init()
{
    // i previously thought the modules moved to within the group_control
    // would need to be initialised HERE. NO! because they MUST have been
    // created prior to the group they are grouped into, and the group
    // MUST have been created BEFORE the group_control - - - -
    // the modules will ALREADY have been initialised (and validated).

    synthmod::list::linkedlist*
        grpmodlist = wcnt::jwm.get_modlist()
            ->remove_modules_of_group(group_name);

    if (!grpmodlist) {
        invalidate();
        return;
    }
    synthmod::list::linkedlist* emptyrunlist =
        move_to_new_list_of_by(grpmodlist, synthmod::base::SM_EMPTY_RUN);
    empty_run_list = move_to_array(emptyrunlist);
    emptyrunlist->empty_list(PRESERVE_DATA);
    delete emptyrunlist;

    runlist = move_to_array(grpmodlist);
    if (!runlist)
        invalidate();
    grpmodlist->empty_list(PRESERVE_DATA);
    delete grpmodlist;
    emptyrunlist = 0;
    grpmodlist = 0;
}

void group_control::run()
{
    if (*in_play_trig == ON) {
        out_play_state = ON;
        stop_pending = false;
    }
    else if (*in_stop_trig == ON)
        stop_pending = wcnt::group_control_stopping_samples;
    else if (stop_pending > 0) {
        stop_pending--;
        if (stop_pending == 0)
            out_play_state = OFF;
    }
    if (out_play_state == ON) {
        long count = 0;
        synthmod::base* sm = runlist[0];
        while(sm) {
            sm->run();
            sm = runlist[++count];
        }
    }
}

bool group_control::set_group_name(const char* name)
{
    if (group_name)
        delete [] group_name;
    group_name = new char[strlen(name) + 1];
    strcpy(group_name, name);

    // group_control must be forced into being created only
    // after the group it is to control has been created.

    dobj::base* grp_dbj =wcnt::get_dobjlist()->get_dobj_by_name(group_name);
    if (!grp_dbj) {
        invalidate();
        sm_err("No group named %s exists.", group_name);
        return false;
    }
    if (grp_dbj->get_object_type() != dobj::DEF_GROUP){
        invalidate();
        sm_err("%s is not a group.", group_name);
        return false;
    }
    grp = (group*)grp_dbj;
    if (grp->is_controlled()) {
        invalidate();
        sm_err("Group %s is already controlled by a group_controller.",
                                                            group_name);
        return false;
    }
    grp->set_controlled();
    return true;

}

