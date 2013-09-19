#include "../include/glame_butterworth.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

glame_butterworth::glame_butterworth(char const* uname) :
 synthmod(synthmodnames::GLAME_BUTTERWORTH, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_phase_step(0), in_res_mod(0), output(0), type(LOPASS),
 resonance(0), res_mod_size(0), l_descriptor(0), l_inst_handle(0),
 l_input(0), l_output(0), l_cut_off(0), l_resonance(0)
{
    register_input(inputnames::IN_SIGNAL);
    register_input(inputnames::IN_CUTOFF_PHASE_STEP);
    register_input(inputnames::IN_RES_MOD);
    register_output(outputnames::OUT_OUTPUT);
    init_first();
    type_names[0] = "buttlow_iir";
    type_names[1] = "butthigh_iir";
    min_cut_off = 0.0001 * jwm.samplerate();
    max_cut_off = 0.45 * jwm.samplerate();
}

glame_butterworth::~glame_butterworth()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_output) delete [] l_output;
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
        sm_err("%s must be within range 0.1 ~ 1.41.",   
                jwm.get_paramnames()->get_name(paramnames::RESONANCE));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void glame_butterworth::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("butterworth_1902",
                                     type_names[type]);
    if (lp == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_inst_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
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

void glame_butterworth::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::GLAME_FILTER_TYPE, "lowpass/highpass");
    register_param(paramnames::RESONANCE);
    register_param(paramnames::RES_MODSIZE);
}

#endif // WITH_LADSPA
