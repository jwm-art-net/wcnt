#include "../include/spreader.h"
#include "../include/dobjmod.h"
#include "../include/dobjlist.h"

#include <math.h>

spreader::spreader(const char* uname) :
 synthmod::base(synthmod::SPREADER, uname, SM_HAS_OUT_OUTPUT),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 in_mod(0), out_output(0), start_level(0), end_level(0), seg_lvl(0),
 sigs(0)
{
    register_output(output::OUT_OUTPUT);
}

void spreader::register_ui()
{
    register_dobj(dobj::LST_SIGNALS, dobj::DOBJ_SYNTHMOD);
    register_input(input::IN_MODULATION);
    register_param(param::START_LEVEL);
    register_param(param::END_LEVEL);
}

ui::moditem_list* spreader::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

spreader::~spreader()
{
    if (sigs)
        delete [] sigs;
}

const void* spreader::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* spreader::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_MODULATION: return in_mod = (double*)o;
        default: return 0;
    }
}

const void* spreader::get_in(int it) const
{
    switch(it)
    {
        case input::IN_MODULATION: return in_mod;
        default: return 0;
    }
}

bool spreader::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::START_LEVEL:
            start_level = *(double*)data;
            return true;
        case param::END_LEVEL:
            end_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* spreader::get_param(int pt) const
{
    switch(pt)
    {
        case param::START_LEVEL:   return &start_level;
        case param::END_LEVEL:     return &end_level;
        default: return 0;
    }
}

synthmod::base* spreader::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

errors::TYPE spreader::validate()
{
    if (!goto_first() || !goto_next()) {
        sm_err("%s", "Must be at least two signals to spread across.");
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}

dobj::base* spreader::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm->flag(SM_HAS_OUT_OUTPUT)) {
            sm_err("%s will not accept the module %s because modules of "
                    "type %s do not have the %s output.",
                    get_username(), sm->get_username(),
                    synthmod::names::get(sm->get_module_type()),
                    output::names::get(output::OUT_OUTPUT));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into spreader.",
                                        sm->get_username());
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        wcnt::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
    return 0;
}

void spreader::init()
{
    sigs = new double const*[get_count() + 1];
    synthmod::base* sm = goto_first();
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
    seg_lvl = (end_level - start_level) / get_count();
}

void spreader::run()
{
    out_output = 0;
    double im_rl = sqrt(*in_mod * *in_mod);
    if (im_rl < start_level)
        im_rl = start_level;
    else if (im_rl > end_level)
        im_rl = end_level;
    long ix = 0;
    double const* o = sigs[0];
    while (o) {
        double lvl = ix * seg_lvl;
        double vol = 0;
        if (im_rl >= lvl && im_rl <= lvl + seg_lvl)
            vol = (seg_lvl - (im_rl - lvl)) / seg_lvl;
        if (im_rl + seg_lvl >= lvl && im_rl + seg_lvl < lvl + seg_lvl)
            vol = (im_rl + seg_lvl - lvl) / seg_lvl;
        out_output += *o * vol;
        o = sigs[++ix];
    }
}

