#ifndef WCNTMODULE_H
#include "../include/wcntexit.h"

wcnt_exit::wcnt_exit(char const* uname) :
 synthmod(synthmodnames::MOD_WCNTEXIT, wcnt_exit_count, uname),
 in_bar(0), exit_bar(0)
{
    get_inputlist()->add_input(this, inputnames::IN_BAR);
    wcnt_exit_count++;
    create_params();
}

wcnt_exit::~wcnt_exit()
{
    get_inputlist()->delete_module_inputs(this);
}

void const* wcnt_exit::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_BAR:
        return in_bar = (short*)o;
    default:
        return 0;
    }
}

void const* wcnt_exit::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_BAR:
        return in_bar;
    default:
        return 0;
    }
}

bool wcnt_exit::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::PAR_EXIT_BAR:
        set_exit_bar(*(short*)data);
        return true;
    default:
        return false;
    }
}

void const* wcnt_exit::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_EXIT_BAR:
        return &exit_bar;
    default:
        return 0;
    }
}

synthmod* wcnt_exit::duplicate_module(const char* uname, DUP_IO dupio)
{
    *err_msg = "wcnt_exit module does not allow copies of it to be made.";
    return 0;
}

stockerrs::ERR_TYPE wcnt_exit::validate()
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

short wcnt_exit::wcnt_exit_count = 0;

bool wcnt_exit::done_params = false;

void wcnt_exit::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_WCNTEXIT, paramnames::PAR_EXIT_BAR);
    done_params = true;
}

#endif
