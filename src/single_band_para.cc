#include "../include/single_band_para.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"

single_band_para::single_band_para(const char* uname) :
 synthmod::base(synthmod::SINGLE_BAND_PARA, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_phase_step(0), in_gain_mod(0), in_bandwidth_mod(0),
 output(0),
 gain_db(0), gain_mod_size(0), bandwidth(0), bandwidth_mod_size(0),
 l_descriptor(0), l_inst_handle(0),
 l_input(0), l_output(0),
 l_gain_db(0), l_frequency(440), l_bandwidth(1)
{
    register_output(output::OUT_OUTPUT);

    max_freq = 0.4 * wcnt::jwm.samplerate();
}

void single_band_para::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_input(input::IN_PHASE_STEP);
    register_param(param::GAIN_DB);
    register_input(input::IN_GAIN_MOD);
    register_param(param::GAIN_MODSIZE);
    register_param(param::BANDWIDTH);
    register_input(input::IN_BANDWIDTH_MOD);
    register_param(param::BANDWIDTH_MODSIZE);
}

single_band_para::~single_band_para()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_output) delete [] l_output;
}

const void* single_band_para::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void*
single_band_para::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:
            return in_signal = (double*)o;
        case input::IN_PHASE_STEP:
            return in_phase_step = (double*)o;
        case input::IN_GAIN_MOD:
            return in_gain_mod = (double*)o;
        case input::IN_BANDWIDTH_MOD:
            return in_bandwidth_mod = (double*)o;
        default:
            return 0;
    }
}

const void* single_band_para::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:         return in_signal;
        case input::IN_PHASE_STEP:     return in_phase_step;
        case input::IN_GAIN_MOD:       return in_gain_mod;
        case input::IN_BANDWIDTH_MOD:  return in_bandwidth_mod;
        default: return 0;
    }
}

bool
single_band_para::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::GAIN_DB:
            gain_db = *(double*)data;
            return true;
        case param::GAIN_MODSIZE:
            gain_mod_size = *(double*)data;
            return true;
        case param::BANDWIDTH:
            bandwidth = *(double*)data;
            return true;
        case param::BANDWIDTH_MODSIZE:
            bandwidth_mod_size = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* single_band_para::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::GAIN_DB:           return &gain_db;
        case param::GAIN_MODSIZE:      return &gain_mod_size;
        case param::BANDWIDTH:         return &bandwidth;
        case param::BANDWIDTH_MODSIZE: return &bandwidth_mod_size;
        default: return 0;
    }
}

errors::TYPE single_band_para::validate()
{
    if (gain_db < -70 || gain_db > 30) {
        sm_err("%s  must be within range -70 ~ 30.",
                    param::names::get(param::GAIN_DB));
        invalidate();
        return errors::ERROR;
    }
    if (bandwidth < 0 || bandwidth > 4) {
        sm_err("%s  must be within range 0 ~ 4.",
                    param::names::get(param::BANDWIDTH));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}

void single_band_para::init()
{
    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
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
    l_frequency = (*in_phase_step * wcnt::jwm.samplerate()) / 360;
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


#endif // WITH_LADSPA
