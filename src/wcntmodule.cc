#ifndef WCNTMODULE_H
#include "../include/wcntmodule.h"

wcnt_module::wcnt_module(char const* uname) :
 synthmod(synthmodnames::MOD_WCNT, wcnt_module_count, uname),
 in_bar(0), exit_bar(0)
{
    get_inputlist()->add_input(this, inputnames::IN_BAR);
    wcnt_module_count++;
    create_params();
}

wcnt_module::~wcnt_module()
{
}

void const* wcnt_module::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_BAR:
        i = in_bar = (short*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool wcnt_module::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_EXIT_BAR:
        set_exit_bar(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* wcnt_module::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_EXIT_BAR:
        return &exit_bar;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE wcnt_module::validate()
{
    if (!get_paramlist()->validate(this, paramnames::PAR_EXIT_BAR,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_EXIT_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

short wcnt_module::wcnt_module_count = 0;

bool wcnt_module::done_params = false;

void wcnt_module::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_WCNT, paramnames::PAR_EXIT_BAR);
    done_params = true;
}

#endif
