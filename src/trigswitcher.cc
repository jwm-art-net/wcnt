#include "../include/trigswitcher.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/dobjdobjlist.h"
#include "../include/duplicate_list_module.h"

trigswitcher::trigswitcher(char const* uname) :

 synthmod(
    synthmodnames::TRIGSWITCHER,
    uname,
    SM_HAS_OUT_TRIG),

 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 in_trig(0), out_trig(OFF),
 trigs(0), trig_ix(0), trig(0)
{
    add_output(outputnames::OUT_TRIG);
    add_input(inputnames::IN_TRIG);
    create_moddobj();
}

trigswitcher::~trigswitcher()
{
    if (trigs)
        delete [] trigs;
}

void const* trigswitcher::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG: return &out_trig;
        default: return 0;
    }
}

void const* trigswitcher::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* trigswitcher::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        default: return 0;
    }
}

synthmod* trigswitcher::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

stockerrs::ERR_TYPE trigswitcher::validate()
{
    if (!goto_first() || !goto_next()) {
        sm_err("%s", "Must be at least two triggers to switch between.");
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* trigswitcher::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm->flag(SM_HAS_OUT_TRIG)) {
            sm_err("%s will not accept the module %s because modules of "
                    "type %s do not have the %s output type.",
                    get_username(), sm->get_username(),
                    jwm.get_modnames()->get_name(sm->get_module_type()),
                    jwm.get_outputnames()->get_name(outputnames::OUT_TRIG));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into trigswitcher.",
                                            sm->get_username());
            return 0;
        }
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("*** MAJOR ERROR *** Bad attempt made to add invalid object "
                                            "type to %s.", get_username());
    return 0;
}

void trigswitcher::init()
{
    trigs = new STATUS const*[get_count() + 1];
    synthmod* sm = goto_first();
    long ix = 0;
    while(sm) {
        trigs[ix] = (STATUS const*)sm->get_out(outputnames::OUT_TRIG);
        sm = goto_next();
        ix++;
    }
    trigs[ix] = 0;
    trig = trigs[trig_ix = 0];
}

void trigswitcher::run()
{
    if (*in_trig == ON) {
        trig_ix++;
        if (trig_ix == get_count())
            trig_ix = 0;
        trig = trigs[trig_ix];
    }
    out_trig = *trig;
}

bool trigswitcher::done_moddobj = false;

void trigswitcher::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::TRIGSWITCHER, dobjnames::LST_TRIGGERS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_TRIGGERS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

