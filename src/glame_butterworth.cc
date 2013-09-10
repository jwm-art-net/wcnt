#ifndef GLAME_BUTTERWORTH_H
#include "../include/glame_butterworth.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

glame_butterworth::glame_butterworth(char const* uname) :
 synthmod(synthmodnames::GLAME_BUTTERWORTH, uname),
 in_signal(0), in_phase_step(0),
 output(0),
 type(LOPASS),
 l_descriptor(0), l_inst_handle(0),
 l_input(0), l_output(0),
 type_names(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist().add_input(this, inputnames::IN_CUTOFF_PHASE_STEP);
    jwm.get_inputlist().add_input(this, inputnames::IN_RES_MOD);
    create_params();
    type_names = new char*[2];
    type_names[0] = "buttlow_iir";
    type_names[1] = "butthigh_iir";
    min_cut_off = 0.0001 * jwm.samplerate();
    max_cut_off = 0.45 * jwm.samplerate();
}

glame_butterworth::~glame_butterworth()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_output) delete [] l_output;
    if (type_names) delete [] type_names;
}

void const* glame_butterworth::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const*
glame_butterworth::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        case inputnames::IN_CUTOFF_PHASE_STEP:
            return in_phase_step = (double*)o;
        case inputnames::IN_RES_MOD:
            return in_res_mod = (double*)o;
        default:
            return 0;
    }
}

void const* glame_butterworth::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:             return in_signal;
        case inputnames::IN_CUTOFF_PHASE_STEP:  return in_phase_step;
        case inputnames::IN_RES_MOD:            return in_res_mod;
        default: return 0;
    }
}

bool
glame_butterworth::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::GLAME_FILTER_TYPE:
            type = *(GB_TYPE*)data;
            return true;
        case paramnames::RESONANCE:
            resonance = *(double*)data;
            return true;
        case paramnames::RES_MODSIZE:
            res_mod_size = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* glame_butterworth::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::GLAME_FILTER_TYPE: return &type;
        case paramnames::RESONANCE:         return &resonance;
        case paramnames::RES_MODSIZE:       return &res_mod_size;
        default: return 0;
    }
}

stockerrs::ERR_TYPE glame_butterworth::validate()
{
    if (resonance < 0.1 || resonance > 1.41) {
        *err_msg +=
         jwm.get_paramnames().get_name(paramnames::RESONANCE);
        *err_msg += " must be within range 0.1 to 1.41";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void glame_butterworth::init()
{
    ladspa_loader& ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll.get_plugin("butterworth_1902",
                                     type_names[type]);
    if (lp == 0) {
        *err_msg = ll.get_error_msg();
        invalidate();
        return;
    }
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        *err_msg = lp->get_error_msg();
        invalidate();
        return;
    }
    if ((l_inst_handle = lp->instantiate()) == 0) {
        *err_msg = lp->get_error_msg();
        invalidate();
        return;
    }
    l_input = new LADSPA_Data[1];
    l_output = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, &l_cut_off);
    l_descriptor->connect_port(l_inst_handle, 1, &l_resonance);
    l_descriptor->connect_port(l_inst_handle, 2, l_input);
    l_descriptor->connect_port(l_inst_handle, 3, l_output);
    l_descriptor->activate(l_inst_handle);
}

void glame_butterworth::run()
{
    *l_input = *in_signal;
    l_cut_off = (*in_phase_step * jwm.samplerate()) / 360;
    if (l_cut_off < min_cut_off) l_cut_off = min_cut_off;
    else if (l_cut_off > max_cut_off) l_cut_off = max_cut_off;
    l_resonance = resonance * (1 - res_mod_size) + resonance *
              *in_res_mod * res_mod_size;
    if (l_resonance < 0.1) l_resonance = 0.1;
    else if(l_resonance > 1.41) l_resonance = 1.41;
    l_descriptor->run(l_inst_handle, 1);
    output = *l_output;
}

bool glame_butterworth::done_params = false;

void glame_butterworth::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
            synthmodnames::GLAME_BUTTERWORTH,
            paramnames::GLAME_FILTER_TYPE);
    jwm.get_fxsparamlist().add_param("lowpass/highpass",
                                  paramnames::GLAME_FILTER_TYPE);
    jwm.get_paramlist().add_param(
            synthmodnames::GLAME_BUTTERWORTH,
            paramnames::RESONANCE);
    jwm.get_paramlist().add_param(
            synthmodnames::GLAME_BUTTERWORTH,
            paramnames::RES_MODSIZE);
    done_params = true;
}

#endif // WITH_LADSPA
#endif // GLAME_BUTTERWORTH_H
