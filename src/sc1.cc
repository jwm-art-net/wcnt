#include "../include/sc1.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

sc1::sc1(const char* uname) :
 synthmod(synthmodnames::SC1, uname, SM_HAS_OUT_OUTPUT),
 input(0), in_thresh_mod(0), in_ratio_mod(0), in_knee_mod(0),
 in_makeup_mod(0), output(0),
 attack(101.5), release(401), thresh(0), ratio(1), knee(3.25), makeup(0),
 l_descriptor(0), l_inst_handle(0),
 l_attack(0), l_release(0), l_thresh(0), l_ratio(0), l_knee(0),
 l_makeup(0), l_input(0), l_output(0)

{
    register_input(inputnames::IN_SIGNAL);
    register_output(outputnames::OUT_OUTPUT);
    init_first();

}

sc1::~sc1()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input)  delete [] l_input;
    if (l_output) delete [] l_output;
}

const void* sc1::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot) {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void* sc1::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it){
        case inputnames::IN_SIGNAL: return input = (double*)o;
        default: return 0;
    }
}

const void* sc1::get_in(inputnames::IN_TYPE it) const
{
    switch(it) {
        case inputnames::IN_SIGNAL: return input;
        default: return 0;
    }
}

bool sc1::set_param(paramnames::PAR_TYPE pt, const void* data)
{
    switch(pt) {
        case paramnames::ATTACK_TIME:
            attack = (*(double*)data);
            return true;
        case paramnames::RELEASE_TIME:
            release = (*(double*)data);
            return true;
        case paramnames::THRESH_DB:
            thresh = (*(double*)data);
            return true;
        case paramnames::RATIO_1N:
            ratio = (*(double*)data);
            return true;
        case paramnames::KNEE_DB:
            knee = (*(double*)data);
            return true;
        case paramnames::MAKEUP_DB:
            makeup = (*(double*)data);
            return true;
        default:
            return false;
    }
}

const void* sc1::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::ATTACK_TIME:   return &attack;
        case paramnames::RELEASE_TIME:  return &release;
        case paramnames::THRESH_DB:     return &thresh;
        case paramnames::RATIO_1N:      return &ratio;
        case paramnames::KNEE_DB:       return &knee;
        case paramnames::MAKEUP_DB:     return &makeup;
        default: return 0;
    }
}

stockerrs::ERR_TYPE sc1::validate()
{
    if (attack  < 2 || attack > 400) {
        sm_err("%s must be within range 2 ~ 400.",
                paramnames::get_name(paramnames::ATTACK_TIME));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (release  < 2 || release > 800) {
        sm_err("%s must be within range 2 ~ 800.",
                paramnames::get_name(paramnames::RELEASE_TIME));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (thresh  < -30 || thresh > 0) {
        sm_err("%s must be within range -30 ~ 0",
                 paramnames::get_name(paramnames::THRESH_DB));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (ratio  < 1 || ratio > 10) {
        sm_err("%s must be within range 1 ~ 10",
                paramnames::get_name(paramnames::RATIO_1N));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (knee  < 1 || knee > 10) {
        sm_err("%s must be within range 1 ~ 10",
                paramnames::get_name(paramnames::KNEE_DB));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (makeup  < 0 || makeup > 24) {
        sm_err("%s must be within range 0 ~ 24",
                paramnames::get_name(paramnames::MAKEUP_DB));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void sc1::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
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

void sc1::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::ATTACK_TIME);
    register_param(paramnames::RELEASE_TIME);
    register_param(paramnames::THRESH_DB);
    register_param(paramnames::RATIO_1N);
    register_param(paramnames::KNEE_DB);
    register_param(paramnames::MAKEUP_DB);
}

#endif // WITH_LADSPA
