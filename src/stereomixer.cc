#ifndef STEREOMIXER_H
#include "../include/stereomixer.h"
#include "../include/groupnames.h"

stereomixer::stereomixer(char const* uname) :
 synthmod(synthmodnames::MOD_STEREOMIXER, stereomixer_count, uname),
 out_left(0), out_right(0), master_level(0.75), o_l(0.00), o_r(0.00),
 chlist(0), chitem(0), chan(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    /* the actual stereomixer module has no inputs */
    chlist =
        new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    stereomixer_count++;
    create_params();
    create_moddobj();
}

stereomixer::~stereomixer()
{
    get_outputlist()->delete_module_outputs(this);
    delete chlist;
}

void const* stereomixer::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_LEFT:
        return &out_left;
    case outputnames::OUT_RIGHT:
        return &out_right;
    default:
        return 0;
    }
}

bool stereomixer::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::PAR_MASTER_LEVEL:
        set_master_level(*(double*)data);
        return true;
    default:
        return false;
    }
}

void const* stereomixer::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_MASTER_LEVEL:
        return &master_level;
    default:
        return 0;
    }
}

synthmod* stereomixer::duplicate_module(const char* uname, DUP_IO dupio)
{
    stereomixer* dup = new stereomixer(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);

    char* current_grp = get_groupname(get_username());
    char* new_grp = get_groupname(uname);
    bool regroup_channels = false;
    if (current_grp && new_grp) {
        if (strcmp(current_grp, new_grp) != 0) {
            regroup_channels = true;
        }
    }
    synthmodlist* modlist = get_modlist();
    if (get_verbose())
        cout << "\n----------\nadding to duplicated mixer " << uname;
    goto_first();
    while (chan) {
        char* chan_grp = get_groupname(chan->get_username());
        synthmod* chan_to_add = chan;
        if (chan_grp && regroup_channels == true) {
            if (strcmp(chan_grp, current_grp) == 0) {
                char* grpchanname =
                        set_groupname(new_grp, chan->get_username());
                synthmod* grpchan =
                            modlist->get_synthmod_by_name(grpchanname);
                if (grpchan) {
                    if (grpchan->get_module_type() ==
                                        synthmodnames::MOD_STEREOCHANNEL)
                        chan_to_add = grpchan;
                    else {
                        cout << "\nin stereomixer::duplicate, an attempt";
                        cout << " to fetch a mix_chan named "
                                                        << grpchanname;
                        cout << "resulted in finding ";
                        cout << grpchan->get_username();
                        cout << " which is not a mix_chan!?!?!";
                    }
                }
                else if (get_verbose()) {
                    cout << "\nWarning! mixer " << uname;
                    cout << " was expecting to find " << grpchanname;
                    cout << " but could not.";
                    cout << "\nCheck the order of grouping in original";
                    cout << " group definition.";
                }
                delete [] grpchanname;
            }
            delete [] chan_grp;
        }
        dup->add_channel((stereo_channel*)chan_to_add);
        if (get_verbose())
            cout << "\nadded " << chan_to_add->get_username();
        goto_next();
    }
    return dup;
}

stockerrs::ERR_TYPE stereomixer::validate()
{
    if (master_level == 0) {
        *err_msg
         = get_paramnames()->get_name(paramnames::PAR_MASTER_LEVEL);
        *err_msg += " is zero, all will be very quiet!";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* stereomixer::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::MOD_STEREOCHANNEL) {
            *err_msg += sm->get_username();
            *err_msg += " is not a stereo_channel";
            return 0;
        }
        if (!add_channel((stereo_channel*)sm)) {
            *err_msg += "could not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into mixer";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        dobj::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += get_username();
    return 0;
}

stereo_channel* stereomixer::add_channel(stereo_channel* ch)
{
    if	(ch == 0)
        return 0;
    if (chlist->add_at_tail(ch) == 0)
    {
        delete chitem;
        return 0;
    }
    return ch;
}

stereo_channel* stereomixer::remove_channel(stereo_channel* ch)
{
    chitem = chlist->find_data(ch);
    if (chitem == 0)
        return 0;
    chan = (stereo_channel*)chlist->unlink_item(chitem)->get_data();
    delete chitem;
    return chan;
}

void stereomixer::run()
{
    o_l = 0.00;
    o_r = 0.00;
    goto_first();
    while(chan)
    {
        o_l += *chan->get_output_left();
        o_r += *chan->get_output_right();
        goto_next();
    }
    o_l *= master_level;
    o_r *= master_level;
    out_left = (short)o_l;
    if (o_l < -32767) out_left = -32767;
    else if (o_l > 32767) out_left = 32767;
    out_right = (short)o_r;
    if (o_r < -32767) out_right = -32767;
    else if (o_r > 32767) out_right = 32767;
}

int stereomixer::stereomixer_count = 0;

bool stereomixer::done_params = false;

void stereomixer::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_STEREOMIXER, paramnames::PAR_MASTER_LEVEL);
    done_params = true;
}

bool stereomixer::done_moddobj = false;

void stereomixer::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_STEREOMIXER, dobjnames::LST_MIX);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_MIX, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
