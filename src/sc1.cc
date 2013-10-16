#include "../include/sc1.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"

sc1::sc1(const char* uname) :
 synthmod::base(synthmod::SC1, uname, SM_HAS_OUT_OUTPUT),
 input(0), in_thresh_mod(0), in_ratio_mod(0), in_knee_mod(0),
 in_makeup_mod(0), output(0),
 attack(101.5), release(401), thresh(0), ratio(1), knee(3.25), makeup(0),
 l_descriptor(0), l_inst_handle(0),
 l_attack(0), l_release(0), l_thresh(0), l_ratio(0), l_knee(0),
 l_makeup(0), l_input(0), l_output(0)
{
    register_output(output::OUT_OUTPUT);
}

void sc1::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::ATTACK_TIME);
    register_param(param::RELEASE_TIME);
    register_param(param::THRESH_DB);
    register_param(param::RATIO_1N);
    register_param(param::KNEE_DB);
    register_param(param::MAKEUP_DB);
}

sc1::~sc1()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input)  delete [] l_input;
    if (l_output) delete [] l_output;
}

const void* sc1::get_out(output::TYPE ot) const
{
    switch(ot) {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void* sc1::set_in(input::TYPE it, const void* o)
{
    switch(it){
        case input::IN_SIGNAL: return input = (double*)o;
        default: return 0;
    }
}

const void* sc1::get_in(input::TYPE it) const
{
    switch(it) {
        case input::IN_SIGNAL: return input;
        default: return 0;
    }
}

bool sc1::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
        case param::ATTACK_TIME:
            attack = (*(double*)data);
            return true;
        case param::RELEASE_TIME:
            release = (*(double*)data);
            return true;
        case param::THRESH_DB:
            thresh = (*(double*)data);
            return true;
        case param::RATIO_1N:
            ratio = (*(double*)data);
            return true;
        case param::KNEE_DB:
            knee = (*(double*)data);
            return true;
        case param::MAKEUP_DB:
            makeup = (*(double*)data);
            return true;
        default:
            return false;
    }
}

const void* sc1::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::ATTACK_TIME:   return &attack;
        case param::RELEASE_TIME:  return &release;
        case param::THRESH_DB:     return &thresh;
        case param::RATIO_1N:      return &ratio;
        case param::KNEE_DB:       return &knee;
        case param::MAKEUP_DB:     return &makeup;
        default: return 0;
    }
}

errors::TYPE sc1::validate()
{
    if (attack  < 2 || attack > 400) {
        sm_err("%s must be within range 2 ~ 400.",
                param::names::get(param::ATTACK_TIME));
        invalidate();
        return errors::ERROR;
    }
    if (release  < 2 || release > 800) {
        sm_err("%s must be within range 2 ~ 800.",
                param::names::get(param::RELEASE_TIME));
        invalidate();
        return errors::ERROR;
    }
    if (thresh  < -30 || thresh > 0) {
        sm_err("%s must be within range -30 ~ 0",
                 param::names::get(param::THRESH_DB));
        invalidate();
        return errors::ERROR;
    }
    if (ratio  < 1 || ratio > 10) {
        sm_err("%s must be within range 1 ~ 10",
                param::names::get(param::RATIO_1N));
        invalidate();
        return errors::ERROR;
    }
    if (knee  < 1 || knee > 10) {
        sm_err("%s must be within range 1 ~ 10",
                param::names::get(param::KNEE_DB));
        invalidate();
        return errors::ERROR;
    }
    if (makeup  < 0 || makeup > 24) {
        sm_err("%s must be within range 0 ~ 24",
                param::names::get(param::MAKEUP_DB));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}

void sc1::init()
{
    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("sc1_1425",
                                     "sc1");
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
    l_descriptor->connect_port(l_inst_handle, 0, &l_attack);
    l_descriptor->connect_port(l_inst_handle, 1, &l_release);
    l_descriptor->connect_port(l_inst_handle, 2, &l_thresh);
    l_descriptor->connect_port(l_inst_handle, 3, &l_ratio);
    l_descriptor->connect_port(l_inst_handle, 4, &l_knee);
    l_descriptor->connect_port(l_inst_handle, 5, &l_makeup);
    l_descriptor->connect_port(l_inst_handle, 6, l_input);
    l_descriptor->connect_port(l_inst_handle, 7, l_output);
    // ___this has no activate function ;-)___
    // initialise unchanging controls...from paramters
    l_attack = attack;
    l_release = release;
    l_thresh = thresh;
    l_ratio = ratio;
    l_knee = knee;
    l_makeup = makeup;
}

void sc1::run()
{
    *l_input  = *input;
    l_descriptor->run(l_inst_handle, 1);
    output = *l_output;
}


#endif // WITH_LADSPA
