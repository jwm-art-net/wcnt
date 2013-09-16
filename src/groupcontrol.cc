#include "../include/groupcontrol.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/group.h"
#include "../include/dobjlist.h"
#include "../include/jwm_init.h"

#include <iostream>
#include <sstream>

group_control::group_control(char const* uname) :

 synthmod(
    synthmodnames::GROUPCONTROL,
    uname,
    SM_UNGROUPABLE | SM_UNDUPLICABLE),

 in_play_trig(0), in_stop_trig(0),
 out_play_state(OFF),
 group_name(0),
 stop_pending(0),
 grp(0), runlist(0), empty_run_list(0)
{
    jwm.get_inputlist()->add_input(this, inputnames::IN_PLAY_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_STOP_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    create_params();
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

const void* group_control::get_out(outputnames::OUT_TYPE ot) const
{
    switch (ot)
    {
        case outputnames::OUT_PLAY_STATE: return &out_play_state;
        default: return 0;
    }
}

void const* group_control::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PLAY_TRIG:
            return in_play_trig = (STATUS const*)o;
        case inputnames::IN_STOP_TRIG:
            return in_stop_trig = (STATUS const*)o;
        default:
            return 0;
    }
}

void const* group_control::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PLAY_TRIG: return in_play_trig;
        case inputnames::IN_STOP_TRIG: return in_stop_trig;
        default: return 0;
    }
}

bool group_control::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::GROUP_NAME:
            return set_group_name((char const*)data);
        default:
            return false;
    }
}

void const* group_control::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::GROUP_NAME: return &group_name;
        default: return 0;
    }
}

stockerrs::ERR_TYPE group_control::validate()
{
    // validation done at set_param level.
    return stockerrs::ERR_NO_ERROR;
}

void group_control::init()
{
    // i previously thought the modules moved to within the group_control
    // would need to be initialised HERE. NO! because they MUST have been
    // created prior to the group they are grouped into, and the group
    // MUST have been created BEFORE the group_control - - - -
    // the modules will ALREADY have been initialised (and validated).

    synthmodlist::linkedlist*
        grpmodlist = jwm.get_modlist()
            ->remove_modules_of_group(group_name);

    if (!grpmodlist) {
        invalidate();
        return;
    }
    synthmodlist::linkedlist*
        emptyrunlist =
            move_to_new_list_of_by(grpmodlist, synthmod::SM_EMPTY_RUN);
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
        stop_pending = jwm_init::group_control_stopping_samples;
    else if (stop_pending > 0) {
        stop_pending--;
        if (stop_pending == 0)
            out_play_state = OFF;
    }
    if (out_play_state == ON) {
        long count = 0;
        synthmod* sm = runlist[0];
        while(sm) {
            sm->run();
            sm = runlist[++count];
        }
    }
}

bool group_control::set_group_name(char const* name)
{
    if (group_name)
        delete [] group_name;
    group_name = new char[strlen(name) + 1];
    strcpy(group_name, name);

    // group_control must be forced into being created only
    // after the group it is to control has been created.

    dobj* grp_dbj = jwm.get_dobjlist()->get_dobj_by_name(group_name);
    if (!grp_dbj) {
        invalidate();
        sm_err("No group named %s exists.", group_name);
        return false;
    }
    if (grp_dbj->get_object_type() != dobjnames::DEF_GROUP){
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

void group_control::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::GROUPCONTROL, paramnames::GROUP_NAME);
}

