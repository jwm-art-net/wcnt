#include "../include/combiner.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/dobjmod.h"
#include "../include/wcntsignal.h"
#include "../include/duplicate_list_module.h"

combiner::combiner(char const* uname) :
 synthmod(synthmodnames::COMBINER, uname, SM_HAS_OUT_OUTPUT),
 linkedlist(MULTIREF_OFF, PRESERVE_DATA),
 out_output(0), meantotal(OFF),
 total(0), sigs(0), sigcount(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    create_params();
    create_moddobj();
}

combiner::~combiner()
{
    if (sigs)
        delete [] sigs;
}

void const* combiner::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

bool combiner::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::MEAN_TOTAL:
            meantotal = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* combiner::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::MEAN_TOTAL: return &meantotal;
        default: return 0;
    }
}

synthmod* combiner::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

stockerrs::ERR_TYPE combiner::validate()
{
    if (!goto_first() || !goto_next()) {
        sm_err("%s", " signals to combine");
        invalidate();
        return stockerrs::ERR_ATLEAST2;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* combiner::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm->flag(SM_HAS_OUT_OUTPUT)) {
            sm_err("%s will not accept the module %s because modules "
                    "of type %s do not have the %s output type.",
                    get_username(), sm->get_username(),
                    jwm.get_modnames()->get_name(sm->get_module_type()),
                    jwm.get_outputnames()->get_name(
                                                outputnames::OUT_OUTPUT));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("It is possible the module %s has already been added",
                                                    sm->get_username());
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

void combiner::init()
{
    sigcount = get_count();
    sigs = new double const*[sigcount + 1];
    synthmod* sm = goto_first();
    long ix = 0;
    while(sm) {
        sigs[ix] = (double const*)sm->get_out(outputnames::OUT_OUTPUT);
        if (!sigs[ix]) {
            sm_err("Things not looking good ;-(... %s",
                   "Don't worry, I have no idea what this means either.");
            invalidate();
            return;
        }
        sm = goto_next();
        ix++;
    }
    sigs[ix] = 0;
    empty_list();
}

void combiner::run()
{
    total = 0;
    long ix = 0;
    double const* o;
    while((o = sigs[ix++]))
        total += *o;
    if (meantotal == ON)
        out_output = total / sigcount;
    else
        out_output = total;
}


void combiner::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->
        add_param(synthmodnames::COMBINER, paramnames::MEAN_TOTAL);
}

bool combiner::done_moddobj = false;
void combiner::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::COMBINER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

