#include "../include/stepdata.h"

step_data::step_data() :
 dobj::base(dobj::SIN_STEP),
 position(0), up_level(0), lo_level(0)
{
}

step_data::step_data(double pos, double ul, double ll) :
 dobj::base(dobj::SIN_STEP),
 position(pos), up_level(ul), lo_level(ll)
{
}

void step_data::register_ui()
{
    register_param(param::POS);
    register_param(param::UPLEVEL);
    register_param(param::LOLEVEL);
}

step_data::~step_data()
{
}

bool step_data::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
        case param::POS:
            position = *(double*)data;
            return true;
        case param::UPLEVEL:
            up_level = *(double*)data;
            return true;
        case param::LOLEVEL:
            lo_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* step_data::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::POS:       return &position;
        case param::UPLEVEL:   return &up_level;
        case param::LOLEVEL:   return &lo_level;
        default: return 0;
    }
}

errors::TYPE step_data::validate()
{
    if (!validate_param(param::POS, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}

