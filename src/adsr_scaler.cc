#ifndef ADSR_SCALER_H
#include "../include/adsr_scaler.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

adsr_scaler::adsr_scaler() :
 dobj(dobjnames::DEF_ADSR_SCALER),
 padsr(0),
 attack_scale(1.0), decay_scale(1.0), release_scale(1.0)
{
    create_params();
}

adsr_scaler::~adsr_scaler()
{
}

bool adsr_scaler::set_param(paramnames::PAR_TYPE pt, void* data)
{
    switch(pt)
    {
        case paramnames::ADSR_NAME:
            if(((synthmod*)data)->get_module_type() ==
               synthmodnames::ADSR)
            {
                padsr = (adsr*)data;
                return true;
            }
            *err_msg = "\n";
            *err_msg += ((synthmod*)data)->get_username();
            *err_msg += " is not an ";
            *err_msg += jwm.get_modnames()->get_name(synthmodnames::ADSR);
            *err_msg += "!";
            return false;
        case paramnames::ATTACK_SCALE:
            attack_scale = *(double*)data;
            return true;
        case paramnames::DECAY_SCALE:
            decay_scale = *(double*)data;
            return true;
        case paramnames::RELEASE_SCALE:
            release_scale = *(double*)data;
            return true;
        default: return false;
    }
}

void const* adsr_scaler::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::ADSR_NAME:     return padsr;
        case paramnames::ATTACK_SCALE:  return &attack_scale;
        case paramnames::DECAY_SCALE:   return &decay_scale;
        case paramnames::RELEASE_SCALE: return &release_scale;
        default: return 0;
    }
}

stockerrs::ERR_TYPE adsr_scaler::validate()
{
    // hang on... the adsr_coords will get validated themselves
    //  - won't they???
    // well no, not if the adsr was defined before the adsr_scaler ;-)
    // (clue: it would have to be).
    if (!jwm.get_dparlist()->validate(
        this, paramnames::ATTACK_SCALE, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(
                                        paramnames::ATTACK_SCALE);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_dparlist()->validate(
        this, paramnames::DECAY_SCALE, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(
                                        paramnames::DECAY_SCALE);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_dparlist()->validate(
        this, paramnames::RELEASE_SCALE, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(
                                        paramnames::RELEASE_SCALE);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    adsr_coord* ac = padsr->goto_first();
    if(!ac){
        // this won't arise because you can't define an adsr with zero
        // sections.
        *err_msg = "\nyeah, so, try harder next time mate.";
        return stockerrs::ERR_ERROR;
    }
    double scale;
    while(ac){
        switch(ac->get_adsr_section()){
            case adsr_coord::ADSR_ATTACK:   scale = attack_scale; break;
            case adsr_coord::ADSR_DECAY:    scale = decay_scale; break;
            case adsr_coord::ADSR_RELEASE:  scale = release_scale; break;
            default: scale = 0; break;
        }
        if (scale > 0){
            ac->set_upper_time(ac->get_upper_time() * scale);
            ac->set_lower_time(ac->get_lower_time() * scale);
        }
        ac = padsr->goto_next();
    }
    return stockerrs::ERR_NO_ERROR;
}

void adsr_scaler::create_params()
{
    if (done_params == true) return;
    jwm.get_dparlist()->add_dobjparam(dobjnames::DEF_ADSR_SCALER,
                                    paramnames::ADSR_NAME);
    jwm.get_dparlist()->add_dobjparam(dobjnames::DEF_ADSR_SCALER,
                                    paramnames::ATTACK_SCALE);
    jwm.get_dparlist()->add_dobjparam(dobjnames::DEF_ADSR_SCALER,
                                    paramnames::DECAY_SCALE);
    jwm.get_dparlist()->add_dobjparam(dobjnames::DEF_ADSR_SCALER,
                                    paramnames::RELEASE_SCALE);
    done_params = true;
}

bool adsr_scaler::done_params = false;

#endif