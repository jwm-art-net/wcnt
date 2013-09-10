#ifndef STEPDATA_H
#include "../include/stepdata.h"

step_data::step_data() :
 dobj(dobjnames::SIN_STEP),
 position(0), up_level(0), lo_level(0)
{
    create_params();
}

step_data::step_data(double pos, double ul, double ll) :
 dobj(dobjnames::SIN_STEP),
 position(pos), up_level(ul), lo_level(ll)
{
    create_params();
}

step_data::~step_data()
{
}

bool step_data::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_POS:
        set_position(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_UPLEVEL:
        set_upper_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_LOLEVEL:
        set_lower_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* step_data::get_param(paramnames::PAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_POS:
        retv = &position;
        break;
    case paramnames::PAR_UPLEVEL:
        retv = &up_level;
        break;
    case paramnames::PAR_LOLEVEL:
        retv = &lo_level;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE step_data::validate()
{
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, paramnames::PAR_POS, stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_POS);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void step_data::create_params()
{
    if (step_data::done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_STEP, paramnames::PAR_POS);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_STEP, paramnames::PAR_UPLEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_STEP, paramnames::PAR_LOLEVEL);
    step_data::done_params = true;
}

bool step_data::done_params = false;

#endif
