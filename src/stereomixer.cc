#ifndef STEREOMIXER_H
#include "../include/stereomixer.h"

stereomixer::stereomixer(char const* uname) :
 synthmod(synthmodnames::MOD_STEREOMIXER, stereomixer_count, uname),
 out_left(0), out_right(0), master_level(0.75), o_l(0.00), o_r(0.00),
 chlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA),
 chitem(0), chan(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
/* cannot register  inputs with modinputslist because they are part of
   list - don't know how many there will be. */
    stereomixer_count++;
    create_params();
    create_moddobj();
}

stereomixer::~stereomixer()
{
    get_outputlist()->delete_module_outputs(this);
    /*no module inputs to delete*/
    /*no need to delete items in channellist as they are just pointers*/
    // to items in another list I think you/I 'll find.
}

void const* stereomixer::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_LEFT:
        o = &out_left;
        break;
    case outputnames::OUT_RIGHT:
        o = &out_right;
        break;
    default:
        o = 0;
    }
    return o;
}

bool stereomixer::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_MASTER_LEVEL:
        set_master_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
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
    if (chlist.add_at_tail(ch) == 0)
    {
        delete chitem;
        return 0;
    }
    return ch;
}

stereo_channel* stereomixer::remove_channel(stereo_channel* ch)
{
    chitem = chlist.find_data(ch);
    if (chitem == 0)
        return 0;
    chan = (stereo_channel*)chlist.unlink_item(chitem)->get_data();
    delete chitem;
    return chan;
}

void stereomixer::run()
{
    o_l = 0.00;
    o_r = 0.00;
    chan = (stereo_channel*)(chitem = chlist.goto_first())->get_data();
    while(chan != 0)
    {
        o_l += *chan->get_output_left();
        o_r += *chan->get_output_right();
        chan = (stereo_channel*)(chitem = chitem->get_next())->get_data();
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
