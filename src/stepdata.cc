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
    register_param(param::LEVEL)->set_flags(ui::UI_OPTION1);
    register_param(param::UPLEVEL)->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::LOLEVEL)->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
}

ui::dobjitem_list* step_data::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

step_data::~step_data()
{
}

bool step_data::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::POS:
            position = *(double*)data;
            return true;
        case param::LEVEL:
            up_level = lo_level = *(double*)data;
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

const void* step_data::get_param(int pt) const
{
    switch(pt)
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

