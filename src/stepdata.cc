#include "../include/stepdata.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

step_data::step_data() :
 dobj(dobjnames::SIN_STEP),
 position(0), up_level(0), lo_level(0)
{
    init_first();
}

step_data::step_data(double pos, double ul, double ll) :
 dobj(dobjnames::SIN_STEP),
 position(pos), up_level(ul), lo_level(ll)
{
    init_first();
}

step_data::~step_data()
{
}

bool step_data::set_param(paramnames::PAR_TYPE dt, void* data)
{
    switch(dt)
    {
        case paramnames::POS:
            position = *(double*)data;
            return true;
        case paramnames::UPLEVEL:
            up_level = *(double*)data;
            return true;
        case paramnames::LOLEVEL:
            lo_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* step_data::get_param(paramnames::PAR_TYPE dt) const
{
    switch(dt)
    {
        case paramnames::POS:       return &position;
        case paramnames::UPLEVEL:   return &up_level;
        case paramnames::LOLEVEL:   return &lo_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE step_data::validate()
{
    if (!jwm.get_dparlist()->validate(
        this, paramnames::POS, stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = paramnames::get_name(paramnames::POS);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void step_data::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::POS);
    register_param(paramnames::UPLEVEL);
    register_param(paramnames::LOLEVEL);
}

