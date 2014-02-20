#include "../include/sc1.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"


ladspa_plug* sc1::lp = 0;


sc1::sc1(const char* uname) :
 synthmod::base(synthmod::SC1, uname, SM_HAS_OUT_OUTPUT),
 input(0), in_thresh_mod(0), in_ratio_mod(0), in_knee_mod(0),
 in_makeup_mod(0), output(0),
 attack(101.5), release(401), thresh(0), ratio(1), knee(3.25), makeup(0),
 l_descriptor(0), l_handle(0),
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

    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    char* path = ll->find_lib_path("sc1_1425");

    if (path) {
        lp = ll->get_plugin(path, "sc1");
        delete [] path;
    }
}


ui::moditem_list* sc1::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}


sc1::~sc1()
{
    if (l_descriptor)
        l_descriptor->cleanup(l_handle);
}


const void* sc1::get_out(int ot) const
{
    switch(ot) {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}


const void* sc1::set_in(int it, const void* o)
{
    switch(it){
        case input::IN_SIGNAL: return input = (double*)o;
        default: return 0;
    }
}


const void* sc1::get_in(int it) const
{
    switch(it) {
        case input::IN_SIGNAL: return input;
        default: return 0;
    }
}


bool sc1::set_param(int pt, const void* data)
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


const void* sc1::get_param(int pt) const
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
    char* err = 0;

    l_attack = attack;
    l_release = release;
    l_thresh = thresh;
    l_ratio = ratio;
    l_knee = knee;
    l_makeup = makeup;

    if ((err = lp->validate_port("Attack time (ms)", &l_attack))) {
        sm_err("%s %s.", param::names::get(param::ATTACK_TIME), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("Release time (ms)", &l_release))) {
        sm_err("%s %s.", param::names::get(param::RELEASE_TIME), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("Threshold level (dB)", &l_thresh))) {
        sm_err("%s %s.", param::names::get(param::THRESH_DB), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("Ratio (1:n)", &l_ratio))) {
        sm_err("%s %s.", param::names::get(param::RATIO_1N), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("Knee radius (dB)", &l_knee))) {
        sm_err("%s %s.", param::names::get(param::KNEE_DB), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("Makeup gain (dB)", &l_makeup))) {
        sm_err("%s %s.", param::names::get(param::MAKEUP_DB), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    return errors::NO_ERROR;
}


void sc1::init()
{
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }

    l_descriptor->connect_port(l_handle, 0, &l_attack);
    l_descriptor->connect_port(l_handle, 1, &l_release);
    l_descriptor->connect_port(l_handle, 2, &l_thresh);
    l_descriptor->connect_port(l_handle, 3, &l_ratio);
    l_descriptor->connect_port(l_handle, 4, &l_knee);
    l_descriptor->connect_port(l_handle, 5, &l_makeup);
    l_descriptor->connect_port(l_handle, 6, &l_input);
    l_descriptor->connect_port(l_handle, 7, &l_output);
    // ___this has no activate function ;-)___
    // initialise unchanging controls...from paramters
}


void sc1::run()
{
    l_input  = *input;
    l_descriptor->run(l_handle, 1);
    output = l_output;
}


#endif // WITH_LADSPA
