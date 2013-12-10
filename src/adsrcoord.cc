#include "../include/adsrcoord.h"

adsr_coord::adsr_coord() :
 dobj::base(dobj::SIN_COORD),
 output_time(0), output_level(0), sect(ADSR_ATTACK),
 upper_time(0), upper_level(0), lower_time(0), lower_level(0)
{
}

adsr_coord::adsr_coord
 (adsr_coord::SECT s, double ut, double ul, double lt, double ll) :
 dobj::base(dobj::SIN_COORD),
 output_time(0), output_level(0), sect(s),
 upper_time(ut), upper_level(ul),
 lower_time(lt), lower_level(ll)
{
}


void adsr_coord::register_ui()
{
    register_param(param::ADSRSECT, "attack/decay/sustain/release");
    register_param(param::UPTIME)->set_flags(ui::UI_OPTION1 | ui::UI_OPT_DUPLICATE);
    register_param(param::LOTIME)->set_flags(ui::UI_OPTION1 | ui::UI_OPT_DUPLICATE);
    register_param(param::TIME, "Specify section time for both upper and lower shapes.")
                                    ->add_descr("section time")
                                    ->set_flags(ui::UI_OPTION2);
    register_param(param::UPLEVEL)->set_flags(ui::UI_OPTION1 | ui::UI_OPT_DUPLICATE);
    register_param(param::LOLEVEL)->set_flags(ui::UI_OPTION1 | ui::UI_OPT_DUPLICATE);
    register_param(param::LEVEL, "Specify section level for both upper and lower shapes.")
                                    ->add_descr("section level")
                                    ->set_flags(ui::UI_OPTION2);
}

ui::dobjitem_list* adsr_coord::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

adsr_coord::~adsr_coord()
{
}

void adsr_coord::run(double velocity)
{
    if (velocity < 0)
        velocity = -velocity;
    if (velocity > 1.0)
        velocity = 1.0;
    output_time = lower_time + (upper_time - lower_time) * velocity;
    output_level = lower_level + (upper_level - lower_level) * velocity;
}

bool adsr_coord::set_param(param::TYPE pt, const void* data)
{
    switch(pt) { // PAR_ADSRSECT is iocat::CAT_FIX_STR.
    case param::ADSRSECT:
        set_adsr_section((SECT)(*(int*)data));
        return true;
    case param::TIME:
        set_upper_time(*(double*)data);
        set_lower_time(*(double*)data);
        return true;
    case param::LEVEL:
        set_upper_level(*(double*)data);
        set_lower_level(*(double*)data);
        return true;
    case param::UPTIME:
        set_upper_time(*(double*)data);
        return true;
    case param::UPLEVEL:
        set_upper_level(*(double*)data);
        return true;
    case param::LOTIME:
        set_lower_time(*(double*)data);
        return true;
    case param::LOLEVEL:
        set_lower_level(*(double*)data);
        return true;
    default:
        return false;
    }
}

const void* adsr_coord::get_param(param::TYPE pt) const
{
    switch(pt) {
    case param::ADSRSECT:  return &sect;
    case param::UPTIME:    return &upper_time;
    case param::UPLEVEL:   return &upper_level;
    case param::LOTIME:    return &lower_time;
    case param::LOLEVEL:   return &lower_level;
    default: return 0;
    }
}

errors::TYPE adsr_coord::validate()
{
    if (!validate_param(param::UPTIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::LOTIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}
