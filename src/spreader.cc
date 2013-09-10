#ifndef SPREADER_H
#include "../include/spreader.h"

spreader::spreader(char const* uname) :
 synthmod(synthmodnames::MOD_SPREADER, spreader_count, uname),
 in_mod(0), out_output(0), start_level(0), end_level(0),
 level_dif(0), sig_count(0), wcntsiglist(0), wcntsig_item(0),
 wcntsig(0), sig(0), prevsig(0), zero(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_MODULATION);
    wcntsiglist =
     new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    spreader_count++;
    create_params();
    create_moddobj();

}

spreader::~spreader()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
/* wcntsig is a synthmodule of type wcnt_signal, created before
    this module was, so don't delete it, idiot. (speaking to myself)
*/
    delete wcntsiglist;
}

void const* spreader::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &out_output;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* spreader::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_MODULATION:
        i = in_mod = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool spreader::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_START_LEVEL:
        set_start_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_END_LEVEL:
        set_end_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* spreader::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_START_LEVEL:
        return &start_level;
    case paramnames::PAR_END_LEVEL:
        return &end_level;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE spreader::validate()
{
    if (!goto_first()) {
        *err_msg = "must be at least two signals to spread across";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        *err_msg= "must be at least two signals to spread across";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* spreader::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
            *err_msg = "\n";
            *err_msg += sm->get_username();
            *err_msg += " is not a wcnt_signal";
            return 0;
        }
        if (!add_signal((wcnt_signal*)sm)) {
            *err_msg = "\ncould not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into spreader";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        dobj::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to " + *get_username();
    return 0;
}

void spreader::init()
{
    goto_first();
    sig_count = 0;
    while (wcntsig) {
        goto_next();
        if (wcntsig)
            sig_count++;
    }
    level_dif = end_level - start_level;
    seg_lvl = level_dif / sig_count;
}

void spreader::run()
{
    out_output = 0;
    short sig_no = 0;
    double im_rl = sqrt(*in_mod * *in_mod);
    if (im_rl < start_level) im_rl = start_level;
    else if (im_rl > end_level) im_rl = end_level;
    goto_first();
    while (wcntsig) {
        double lvl = sig_no * seg_lvl;
        double vol = 0;
        if (im_rl >= lvl && im_rl <= lvl + seg_lvl)
            vol = (seg_lvl - (im_rl - lvl)) / seg_lvl;
        if (im_rl + seg_lvl >= lvl && im_rl + seg_lvl < lvl + seg_lvl)
            vol = (im_rl + seg_lvl - lvl) / seg_lvl;
        out_output += *(wcntsig->get_output()) * vol;
        sig_no++;
        goto_next();
    }
}

int spreader::spreader_count = 0;

bool spreader::done_params = false;

void spreader::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SPREADER, paramnames::PAR_START_LEVEL);
    get_paramlist()->add_param(
     synthmodnames::MOD_SPREADER, paramnames::PAR_END_LEVEL);
    done_params = true;
}

bool spreader::done_moddobj = false;

void spreader::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_SPREADER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
