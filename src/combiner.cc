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

combiner::combiner(const char* uname) :
 synthmod(module::COMBINER, uname, SM_HAS_OUT_OUTPUT),
 linkedlist(MULTIREF_OFF, PRESERVE_DATA),
 out_output(0), meantotal(OFF),
 total(0), sigs(0), sigcount(0)
{
    register_output(output::OUT_OUTPUT);
    init_first();
}

combiner::~combiner()
{
    if (sigs)
        delete [] sigs;
}

const void* combiner::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

bool combiner::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::MEAN_TOTAL:
            meantotal = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* combiner::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::MEAN_TOTAL: return &meantotal;
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
                    module::names::get(sm->get_module_type()),
                    output::names::get(
                                                output::OUT_OUTPUT));
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
    sm_err("%s %s to %s", stockerrs::major, stockerrs::bad_add,
                                                    get_username());
    return 0;
}

void combiner::init()
{
    sigcount = get_count();
    sigs = new double const*[sigcount + 1];
    synthmod* sm = goto_first();
    long ix = 0;
    while(sm) {
        sigs[ix] = (double const*)sm->get_out(output::OUT_OUTPUT);
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


void combiner::init_first()
{
    if (done_first())
        return;
    register_param(param::MEAN_TOTAL);
    register_moddobj(dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
}

