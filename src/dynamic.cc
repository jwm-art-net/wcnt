#include "../include/dynamic.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

dynamic::dynamic(char const* uname) :
 synthmod(synthmodnames::DYNAMIC, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_mod(0), out_output(0), play_state(OFF),
 up_thresh(0), lo_thresh(0), posnegmirror(OFF), use_ratios(OFF),
 dynvertices(0), dvc(0), dvn(0),
 thresh_range(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist()->add_input(this, inputnames::IN_MODULATION);
    create_params();
    create_dobj();
}

dynamic::~dynamic()
{
    if (dynvertices)
        destroy_array_moved_from_list(dynvertices);
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
    switch(dbj->get_object_type())
    {
        case dobjnames::SIN_DVERTEX:
            if (!(retv = add_dvertex((dynvertex*)dbj)))
                *err_msg="\ncould not add vertex to " + *get_username();
        break;
        default:
            *err_msg = "\n***major error*** attempt made to add an ";
            *err_msg += "\ninvalid object type to " + *get_username();
    }
    return retv;
}

synthmod* dynamic::duplicate_module(const char* uname, DUP_IO dupio)
{
    dynamic* dup = new dynamic(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    dvc = goto_first();
    while(dvc) {
        dup->add_dvertex(   dvc->get_signal_in_level(),
                            dvc->get_upper_signal_out_level(),
                            dvc->get_lower_signal_out_level());
        dvc = goto_next();
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
         jwm.get_paramnames()->get_name(paramnames::UP_THRESH);
        *err_msg += " must not be less than ";
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::LO_THRESH);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dynvertex* dynamic::add_dvertex(double sil, double usol, double lsol)
{
    dvc = new dynvertex(sil, usol, lsol);
    if (!add_dvertex(dvc)) {
        delete dvc;
        return 0;
    }
    return dvc;
}

bool dynamic::delete_dvertex(dynvertex* dv)
{
    if (!dv)
        return false;
    if (dv == sneak_first()->get_data() ||
        dv == sneak_last()->get_data())
        // don't allow user to delete initial or end
        return false;
    llitem* tmp = find_data(sneak_first(), dv);
    if (tmp == 0)
        return false;
    delete unlink_item(tmp);
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
    if (!(dynvertices = move_to_array(this)))
        invalidate();
}

dynvertex* dynamic::goto_dvertex(double sil)
{
    long dvix = 0;
    dvc = dynvertices[dvix];
    dvn = 0;
    while (dvc) {
        if ((dvn = dynvertices[dvix + 1])) {
            if (!(dvc->get_signal_in_level()
                    == dvn->get_signal_in_level()))
            {
                if (sil >= dvc->get_signal_in_level() &&
                        sil < dvn->get_signal_in_level())
                    return dvn;
            }
        } else
            return 0;
        dvc = dynvertices[++dvix];
    }
    return 0;
}

void dynamic::run()
{
    double isig = *in_signal;
    double insig = isig;
    double insig_sign = (insig < 0) ? -1 : 1;

    if (posnegmirror == ON)
        insig *= insig_sign;
    else
        insig_sign = 1;

    dvn = goto_dvertex(insig);

    double sil = dvc->get_signal_in_level();
    double inmod = *in_mod;

    inmod *= (inmod < 0) ? -1 : 1;

    double m = (inmod - lo_thresh) / thresh_range;

    if (m <= 0.0)
        m = 0;
    else if (m > 1.0)
        m = 1.0;

    double ol = dvc->get_out_level(m);
    if (dvn) {
        out_output = (ol + (dvn->get_out_level(m) - ol) 
         * ((insig - sil) / (dvn->get_signal_in_level() - sil)))
         * ((use_ratios == OFF) ? insig_sign : isig);
    }
    else out_output = ol * ((use_ratios == OFF) ? insig_sign : isig);
}


bool dynamic::done_dobj = false;

void dynamic::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::DYNAMIC, paramnames::UP_THRESH);
    jwm.get_paramlist()->add_param(
     synthmodnames::DYNAMIC, paramnames::LO_THRESH);
    jwm.get_paramlist()->add_param(
     synthmodnames::DYNAMIC, paramnames::POSNEG_MIRROR);
    jwm.get_paramlist()->add_param(
     synthmodnames::DYNAMIC, paramnames::USE_RATIOS);
    
}

void dynamic::create_dobj()
{
    if (done_dobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::DYNAMIC, dobjnames::LST_DYNAMICS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_DYNAMICS, dobjnames::SIN_DVERTEX);
    done_dobj = true;
}
