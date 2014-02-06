#include "../include/balance.h"

balance::balance(const char* uname) :
 synthmod::base(synthmod::BALANCE, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0),out_output(0), func(ADD), bias(0.5)
{
    register_output(output::OUT_OUTPUT);
}

void balance::register_ui()
{
    register_input(input::IN_SIGNAL1);
    register_param(param::FUNC, "add|sub|mul");
    register_input(input::IN_SIGNAL2);
    register_param(param::BIAS)->set_flags(ui::UI_OPTIONAL);
}

ui::moditem_list* balance::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}


balance::~balance()
{
}

const void* balance::get_out(int ot) const
{
    return (ot == output::OUT_OUTPUT ? &out_output : 0);
}

const void* balance::set_in(int it, const void* o)
{
    switch(it) {
    case input::IN_SIGNAL1: return in_signal1 = (double*)o;
    case input::IN_SIGNAL2: return in_signal2 = (double*)o;
    default: return 0;
    }
}

const void* balance::get_in(int it) const
{
    switch(it) {
    case input::IN_SIGNAL1: return in_signal1;
    case input::IN_SIGNAL2: return in_signal2;
    default: return 0;
    }
}

bool balance::set_param(int pt, const void* data)
{
    switch(pt) {
    case param::FUNC:
        func = (FUNC)(*(int*)data);
        return true;
    case param::BIAS:
        bias = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* balance::get_param(int pt) const
{
    switch(pt) {
    case param::FUNC: return &func;
    case param::BIAS: return &bias;
    default: return 0;
    }
}

errors::TYPE balance::validate()
{
    if (!validate_param(param::BIAS, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}

void balance::run()
{
    if (func == ADD)
        out_output = *in_signal1 * (1 - bias) + *in_signal2 * bias;
    else if (func == SUB)
        out_output = *in_signal1 * (1 - bias) - *in_signal2 * bias;
    else
        out_output = *in_signal1 * (1 - bias)
            + *in_signal2 * bias * *in_signal1;
}
