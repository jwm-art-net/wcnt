#ifndef WCNTEXIT_H
#include "../include/wcntexit.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

wcnt_exit::wcnt_exit(char const* uname) :

 synthmod(
    synthmodnames::WCNTEXIT,
    uname,
    SM_EMPTY_RUN | SM_UNGROUPABLE | SM_UNDUPLICABLE),

 in_bar(0), exit_bar(0)
{
    jwm.get_inputlist()->add_input(this, inputnames::IN_BAR);
    create_params();
}

wcnt_exit::~wcnt_exit()
{
}

void const* wcnt_exit::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_BAR: return in_bar = (short*)o;
        default: return 0;
    }
}

void const* wcnt_exit::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_BAR: return in_bar;
        default: return 0;
    }
}

bool wcnt_exit::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::EXIT_BAR:
            exit_bar = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* wcnt_exit::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::EXIT_BAR: return &exit_bar;
        default: return 0;
    }
}

synthmod* wcnt_exit::duplicate_module(const char* uname, DUP_IO dupio)
{
    *err_msg = "wcnt_exit module does not allow copies of it to be made.";
    return 0;
}

stockerrs::ERR_TYPE wcnt_exit::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::EXIT_BAR,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::EXIT_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wcnt_exit::init()
{
    jwm.x_exit_bar = exit_bar;
    jwm.x_in_bar = in_bar;
}

bool wcnt_exit::done_params = false;

void wcnt_exit::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::WCNTEXIT, paramnames::EXIT_BAR);
    done_params = true;
}

#endif
