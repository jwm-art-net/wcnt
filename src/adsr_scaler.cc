#include "../include/adsr_scaler.h"
#include "../include/globals.h"

adsr_scaler::adsr_scaler() :
 dobj::base(dobj::DEF_ADSR_SCALER),
 padsr(0),
 attack_scale(1.0), decay_scale(1.0), release_scale(1.0)
{
}

void adsr_scaler::register_ui()
{
    register_param(param::ADSR_NAME);
    register_param(param::ATTACK_SCALE);
    register_param(param::DECAY_SCALE);
    register_param(param::RELEASE_SCALE);
}

adsr_scaler::~adsr_scaler()
{
}

bool adsr_scaler::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
    case param::ADSR_NAME:
        if (((synthmod::base*)data)->get_module_type() == synthmod::ADSR) {
            padsr = (adsr*)data;
            return true;
        }
        dobjerr("%s is not an %s.", ((synthmod::base*)data)->get_username(),
                                      synthmod::names::get(synthmod::ADSR));
        return false;
    case param::ATTACK_SCALE:
        attack_scale = *(double*)data;
        return true;
    case param::DECAY_SCALE:
        decay_scale = *(double*)data;
        return true;
    case param::RELEASE_SCALE:
        release_scale = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* adsr_scaler::get_param(param::TYPE pt) const
{
    switch(pt) {
    case param::ADSR_NAME:     return padsr;
    case param::ATTACK_SCALE:  return &attack_scale;
    case param::DECAY_SCALE:   return &decay_scale;
    case param::RELEASE_SCALE: return &release_scale;
    default: return 0;
    }
}

errors::TYPE adsr_scaler::validate()
{
    // hang on... the adsr_coords will get validated themselves
    //  - won't they???
    // well no, not if the adsr was defined before the adsr_scaler ;-)
    // (clue: it would have to be).
    if (!validate_param(param::ATTACK_SCALE, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::DECAY_SCALE, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::RELEASE_SCALE, errors::NEGATIVE))
        return errors::NEGATIVE;

    adsr_coord* ac = padsr->goto_first();
    if(!ac){
        // this won't arise because you can't define an adsr with zero
        // sections.
        dobjerr("%s", "The impossible has happened.");
        return errors::ERROR;
    }
    double scale;
    while(ac) {
        switch(ac->get_adsr_section()) {
        case adsr_coord::ADSR_ATTACK:   scale = attack_scale; break;
        case adsr_coord::ADSR_DECAY:    scale = decay_scale; break;
        case adsr_coord::ADSR_RELEASE:  scale = release_scale; break;
        default: scale = 0; break;
        }
        if (scale > 0) {
            ac->set_upper_time(ac->get_upper_time() * scale);
            ac->set_lower_time(ac->get_lower_time() * scale);
        }
        ac = padsr->goto_next();
    }
    return errors::NO_ERROR;
}


