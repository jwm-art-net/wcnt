#ifndef DYNAMIC_H
#include "../include/dynamic.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

dynamic::dynamic(char const* uname) :
 synthmod(synthmodnames::DYNAMIC, uname),
 in_signal(0), in_mod(0), out_output(0), play_state(OFF),
 up_thresh(0), lo_thresh(0), posnegmirror(OFF), use_ratios(OFF),
 dynamics(0), dvtx_item(0), dvtx_item1(0), dvtx(0), dvtx1(0),
 thresh_range(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist().add_output(this, outputnames::OUT_PLAY_STATE);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist().add_input(this, inputnames::IN_MODULATION);
    dynamics = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    create_params();
    create_dobj();
}

dynamic::~dynamic()
{
    goto_first();
    while (dvtx)
    {
        dvtx1 = dvtx;
        goto_next();
        delete dvtx1;
    }
    delete dynamics;
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
}

void const* dynamic::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &out_output;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

void const* dynamic::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:     return in_signal = (double*)o;
        case inputnames::IN_MODULATION: return in_mod = (double*)o;
        default: return 0;
    }
}

void const* dynamic::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:     return in_signal;
        case inputnames::IN_MODULATION: return in_mod;
        default: return 0;
    }
}

bool dynamic::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::UP_THRESH:
            up_thresh = *(double*)data;
            return true;
        case paramnames::LO_THRESH:
            lo_thresh = *(double*)data;
            return true;
        case paramnames::POSNEG_MIRROR:
            posnegmirror = *(STATUS*)data;
            return true;
        case paramnames::USE_RATIOS:
            use_ratios = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* dynamic::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::UP_THRESH:     return &up_thresh;
        case paramnames::LO_THRESH:     return &lo_thresh;
        case paramnames::POSNEG_MIRROR: return &posnegmirror;
        case paramnames::USE_RATIOS:    return &use_ratios;
        default: return 0;
    }
}

dobj* dynamic::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_DVERTEX:
        if (!(retv = add_dvertex((dynvertex*)dbj)))
            *err_msg="\ncould not add vertex to " + *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to " + *get_username();
        retv = 0;
    }
    return retv;
}

synthmod* dynamic::duplicate_module(const char* uname, DUP_IO dupio)
{
    dynamic* dup = new dynamic(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    goto_first();
    while(dvtx) {
        dup->add_dvertex(dvtx->get_signal_in_level(),
                            dvtx->get_upper_signal_out_level(),
                            dvtx->get_lower_signal_out_level());
        goto_next();
    }
    return dup;
}

stockerrs::ERR_TYPE dynamic::validate()
{
    if (!goto_first()) {
        *err_msg = "***oops*** try adding some dvertex to amp_map";
        invalidate();
        return stockerrs::ERR_ERROR;
    } else if (!goto_next()) {
        *err_msg =
         "***oops*** try adding more than one dvertex to amp_map";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (up_thresh < lo_thresh) {
        *err_msg =
         jwm.get_paramnames().get_name(paramnames::UP_THRESH);
        *err_msg += " must not be less than ";
        *err_msg +=
         jwm.get_paramnames().get_name(paramnames::LO_THRESH);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dynvertex* dynamic::add_dvertex(dynvertex* dv)
{
    if (!dv) return 0;
    return ordered_insert(dynamics, dv, &dynvertex::get_signal_in_level);
}

dynvertex* dynamic::add_dvertex(double sil, double usol, double lsol)
{
    dvtx1 = new dynvertex(sil, usol, lsol);
    if (!ordered_insert(dynamics, dvtx1,
        &dynvertex::get_signal_in_level))
    {
        delete dvtx1;
        return 0;
    }
    return dvtx1;
}

bool dynamic::delete_dvertex(dynvertex* dv)
{
    if (!dv)
        return false;
    if (dv == get_first() || dv == get_last())
        return false;  // don't allow user to delete initial or end

    ll_item* tmp = dynamics->find_data(dv);
    if (tmp == 0)
        return false;
    delete dynamics->unlink_item(tmp);
    delete dv;
    return true;
}

void dynamic::init()
{
    if (up_thresh < lo_thresh) {
        double ut = up_thresh;
        up_thresh = lo_thresh;
        lo_thresh = ut;
    }
    thresh_range = up_thresh - lo_thresh;
}

dynvertex* dynamic::goto_dvertex(double sil)
{
    goto_first();
    while (dvtx) {
        if ((dvtx1 = get_next())) {
            if (!(dvtx->get_signal_in_level()
                    == dvtx1->get_signal_in_level()))
                if (sil >= dvtx->get_signal_in_level() &&
                        sil < dvtx1->get_signal_in_level())
                    return dvtx1;
        } else
            return 0;
        goto_next();
    }
    return 0;
}

void dynamic::run()
{
    double isig = *in_signal;
    double insig = isig;
    double insig_sign = (insig < 0) ? -1 : 1;
    if (posnegmirror == ON) insig *= insig_sign; else insig_sign = 1;
    dvtx1 = goto_dvertex(insig);
    double sil = dvtx->get_signal_in_level();
    double inmod = *in_mod;
    inmod *= (inmod < 0) ? -1 : 1;
    double m = (inmod - lo_thresh) / thresh_range;
    if (m <= 0.0) m = 0; else if (m > 1.0) m = 1.0;
    double ol = dvtx->get_out_level(m);
    if (dvtx1) {
        out_output = (ol + (dvtx1->get_out_level(m) - ol) 
         * ((insig - sil) / (dvtx1->get_signal_in_level() - sil)))
         * ((use_ratios == OFF) ? insig_sign : isig);
    }
    else out_output = ol * ((use_ratios == OFF) ? insig_sign : isig);
}

bool dynamic::done_params = false;
bool dynamic::done_dobj = false;

void dynamic::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
     synthmodnames::DYNAMIC, paramnames::UP_THRESH);
    jwm.get_paramlist().add_param(
     synthmodnames::DYNAMIC, paramnames::LO_THRESH);
    jwm.get_paramlist().add_param(
     synthmodnames::DYNAMIC, paramnames::POSNEG_MIRROR);
    jwm.get_paramlist().add_param(
     synthmodnames::DYNAMIC, paramnames::USE_RATIOS);
    done_params = true;
}

void dynamic::create_dobj()
{
    if (done_dobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::DYNAMIC, dobjnames::LST_DYNAMICS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_DYNAMICS, dobjnames::SIN_DVERTEX);
    done_dobj = true;
}
#endif
