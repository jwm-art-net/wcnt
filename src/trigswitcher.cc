#include "../include/trigswitcher.h"
#include "../include/dobjmod.h"
#include "../include/dobjlist.h"

trigswitcher::trigswitcher(const char* uname) :

 synthmod::base(synthmod::TRIGSWITCHER, uname, SM_HAS_OUT_TRIG),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 in_trig(0), out_trig(OFF),
 trigs(0), trig_ix(0), trig(0)
{
    register_output(output::OUT_TRIG);
}

void trigswitcher::register_ui()
{
    register_input(input::IN_TRIG);
    register_dobj(dobj::LST_TRIGGERS, dobj::DOBJ_SYNTHMOD);
}

trigswitcher::~trigswitcher()
{
    if (trigs)
        delete [] trigs;
}

const void* trigswitcher::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG: return &out_trig;
        default: return 0;
    }
}

const void* trigswitcher::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

const void* trigswitcher::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig;
        default: return 0;
    }
}

synthmod::base* trigswitcher::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

errors::TYPE trigswitcher::validate()
{
    if (!goto_first() || !goto_next()) {
        sm_err("%s", "Must be at least two triggers to switch between.");
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}

dobj::base* trigswitcher::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm->flag(SM_HAS_OUT_TRIG)) {
            sm_err("%s will not accept the module %s because modules of "
                    "type %s do not have the %s output type.",
                    get_username(), sm->get_username(),
                    synthmod::names::get(sm->get_module_type()),
                    output::names::get(output::OUT_TRIG));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into trigswitcher.",
                                            sm->get_username());
            return 0;
        }
       wcnt::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
    return 0;
}

void trigswitcher::init()
{
    trigs = new STATUS const*[get_count() + 1];
    synthmod::base* sm = goto_first();
    long ix = 0;
    while(sm) {
        trigs[ix] = (STATUS const*)sm->get_out(output::OUT_TRIG);
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

