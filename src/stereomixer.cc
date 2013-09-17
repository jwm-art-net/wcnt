#include "../include/stereomixer.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/dobjdobjlist.h"
#include "../include/duplicate_list_module.h"

stereomixer::stereomixer(char const* uname) :
 synthmod(synthmodnames::STEREOMIXER, uname, SM_HAS_STEREO_OUTPUT),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 out_left(0), out_right(0), master_level(0.75),
 chans_left(0), chans_right(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    create_params();
    create_moddobj();
}

stereomixer::~stereomixer()
{
    if (chans_left)
        delete [] chans_left;
    if (chans_right)
        delete [] chans_right;
}

void const* stereomixer::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT:  return &out_left;
        case outputnames::OUT_RIGHT: return &out_right;
        default: return 0;
    }
}

bool stereomixer::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::MASTER_LEVEL:
            master_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* stereomixer::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::MASTER_LEVEL: return &master_level;
        default: return 0;
    }
}

synthmod* stereomixer::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

stockerrs::ERR_TYPE stereomixer::validate()
{
    if (master_level == 0) {
        // should probably allow zero amplitude...
        sm_err("%s is zero, all will be very quiet!",
                jwm.get_paramnames()->get_name(paramnames::MASTER_LEVEL));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* stereomixer::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::STEREOCHANNEL
            && !sm->get_out(outputnames::OUT_LEFT))
        {
            sm_err("%s will not accept the module %s because modules of "
                    "type %s do not have the %s or %s output types.",
                    get_username(), sm->get_username(),
                    jwm.get_modnames()->get_name(sm->get_module_type()),
                    jwm.get_outputnames()->get_name(outputnames::OUT_LEFT),
                    jwm.get_outputnames()->get_name(
                                                outputnames::OUT_RIGHT));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into mixer.", sm->get_username());
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("*** MAJOR ERROR *** Bad attempt made to add invalid object "
                                            "type to %s.", get_username());
    return 0;
}

void stereomixer::init()
{
    chans_left  = new double const*[get_count() + 1];
    chans_right = new double const*[get_count()];
    synthmod* sm = goto_first();
    long ix = 0;
    while(sm) {
        chans_left[ix] =
            (double const*)sm->get_out(outputnames::OUT_LEFT);
        chans_right[ix] =
            (double const*)sm->get_out(outputnames::OUT_RIGHT);
        ix++;
        sm = goto_next();
    }
    chans_left[ix] = 0;
    empty_list();
}

void stereomixer::run()
{
    out_left = out_right = 0;
    for (long ix = 0; chans_left[ix]; ix++){
        out_left  += *chans_left[ix];
        out_right += *chans_right[ix];
    }
    out_left  *= master_level;
    out_right *= master_level;
}

void stereomixer::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::MASTER_LEVEL);
}

bool stereomixer::done_moddobj = false;

void stereomixer::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::STEREOMIXER, dobjnames::LST_MIX);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_MIX, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

