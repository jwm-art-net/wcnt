#include "../include/single_band_para.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

single_band_para::single_band_para(char const* uname) :
 synthmod(synthmodnames::SINGLE_BAND_PARA, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_phase_step(0), in_gain_mod(0), in_bandwidth_mod(0),
 output(0),
 gain_db(0), gain_mod_size(0), bandwidth(0), bandwidth_mod_size(0),
 l_descriptor(0), l_inst_handle(0),
 l_input(0), l_output(0),
 l_gain_db(0), l_frequency(440), l_bandwidth(1)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist()->add_input(this, inputnames::IN_PHASE_STEP);
    jwm.get_inputlist()->add_input(this, inputnames::IN_GAIN_MOD);
    jwm.get_inputlist()->add_input(this, inputnames::IN_BANDWIDTH_MOD);
    create_params();
    max_freq = 0.4 * jwm.samplerate();
}

single_band_para::~single_band_para()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_output) delete [] l_output;
}

void const* single_band_para::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const*
single_band_para::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        case inputnames::IN_PHASE_STEP:
            return in_phase_step = (double*)o;
        case inputnames::IN_GAIN_MOD:
            return in_gain_mod = (double*)o;
        case inputnames::IN_BANDWIDTH_MOD:
            return in_bandwidth_mod = (double*)o;
        default:
            return 0;
    }
}

void const* single_band_para::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:         return in_signal;
        case inputnames::IN_PHASE_STEP:     return in_phase_step;
        case inputnames::IN_GAIN_MOD:       return in_gain_mod;
        case inputnames::IN_BANDWIDTH_MOD:  return in_bandwidth_mod;
        default: return 0;
    }
}

bool
single_band_para::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::GAIN_DB:
            gain_db = *(double*)data;
            return true;
        case paramnames::GAIN_MODSIZE:
            gain_mod_size = *(double*)data;
            return true;
        case paramnames::BANDWIDTH:
            bandwidth = *(double*)data;
            return true;
        case paramnames::BANDWIDTH_MODSIZE:
            bandwidth_mod_size = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* single_band_para::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::GAIN_DB:           return &gain_db;
        case paramnames::GAIN_MODSIZE:      return &gain_mod_size;
        case paramnames::BANDWIDTH:         return &bandwidth;
        case paramnames::BANDWIDTH_MODSIZE: return &bandwidth_mod_size;
        default: return 0;
    }
}

stockerrs::ERR_TYPE single_band_para::validate()
{
    if (gain_db < -70 || gain_db > 30) {
        sm_err("%s  must be within range -70 ~ 30.",
                    jwm.get_paramnames()->get_name(paramnames::GAIN_DB));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (bandwidth < 0 || bandwidth > 4) {
        sm_err("%s  must be within range 0 ~ 4.",
                    jwm.get_paramnames()->get_name(paramnames::BANDWIDTH));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void single_band_para::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("single_para_1203", "singlePara");
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
    l_descriptor->connect_port(l_inst_handle, 0, &l_gain_db);
    l_descriptor->connect_port(l_inst_handle, 1, &l_frequency);
    l_descriptor->connect_port(l_inst_handle, 2, &l_bandwidth);
    l_descriptor->connect_port(l_inst_handle, 3, l_input);
    l_descriptor->connect_port(l_inst_handle, 4, l_output);
    l_descriptor->activate(l_inst_handle);
}

void single_band_para::run()
{
    *l_input = *in_signal;
    l_frequency = (*in_phase_step * jwm.samplerate()) / 360;
    if (l_frequency < 0) l_frequency = 0;
    else if (l_frequency > max_freq) l_frequency = max_freq;
    l_gain_db = gain_db * (1 - gain_mod_size) + gain_db *
              *in_gain_mod * gain_mod_size;
    if (l_gain_db < -70) l_gain_db = -70;
    else if(l_gain_db > 30) l_gain_db = 30;
    l_bandwidth = bandwidth * (1 - bandwidth_mod_size) + bandwidth *
              *in_bandwidth_mod * bandwidth_mod_size;
    if (l_bandwidth < 0) l_bandwidth = 0;
    else if(l_bandwidth > 4) l_bandwidth = 4;
    l_descriptor->run(l_inst_handle, 1);
    output = *l_output;
}

void single_band_para::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::GAIN_DB);
    relate_param(paramnames::GAIN_MODSIZE);
    relate_param(paramnames::BANDWIDTH);
    relate_param(paramnames::BANDWIDTH_MODSIZE);
}

#endif // WITH_LADSPA
