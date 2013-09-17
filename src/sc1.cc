#include "../include/sc1.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

sc1::sc1(char const* uname) :
 synthmod(synthmodnames::SC1, uname, SM_HAS_OUT_OUTPUT),
 input(0), in_thresh_mod(0), in_ratio_mod(0), in_knee_mod(0),
 in_makeup_mod(0), output(0),
 attack(101.5), release(401), thresh(0), ratio(1), knee(3.25), makeup(0),
 l_descriptor(0), l_inst_handle(0),
 l_attack(0), l_release(0), l_thresh(0), l_ratio(0), l_knee(0),
 l_makeup(0), l_input(0), l_output(0)

{
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    create_params();

}

sc1::~sc1()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input)  delete [] l_input;
    if (l_output) delete [] l_output;
}

void const* sc1::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot) {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const* sc1::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it){
        case inputnames::IN_SIGNAL: return input = (double*)o;
        default: return 0;
    }
}

void const* sc1::get_in(inputnames::IN_TYPE it) const
{
    switch(it) {
        case inputnames::IN_SIGNAL: return input;
        default: return 0;
    }
}

bool sc1::set_param(paramnames::PAR_TYPE pt, void const* data)
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

void const* sc1::get_param(paramnames::PAR_TYPE pt) const
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
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::ATTACK_TIME);
        *err_msg += " must be within range 2 to 400";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (release  < 2 || release > 800) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::RELEASE_TIME);
        *err_msg += " must be within range 2 to 800";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (thresh  < -30 || thresh > 0) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::THRESH_DB);
        *err_msg += " must be within range -30 to 0";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (ratio  < 1 || ratio > 10) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::RATIO_1N);
        *err_msg += " must be within range 1 to 10";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (knee  < 1 || knee > 10) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::KNEE_DB);
        *err_msg += " must be within range 1 to 10";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (makeup  < 0 || makeup > 24) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::MAKEUP_DB);
        *err_msg += " must be within range 1 to 10";
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
        *err_msg = ll->get_error_msg();
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

void sc1::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::ATTACK_TIME);
    relate_param(paramnames::RELEASE_TIME);
    relate_param(paramnames::THRESH_DB);
    relate_param(paramnames::RATIO_1N);
    relate_param(paramnames::KNEE_DB);
    relate_param(paramnames::MAKEUP_DB);
}

#endif // WITH_LADSPA
