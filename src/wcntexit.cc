#include "../include/wcntexit.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/connectorlist.h"

#include <string.h>

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
    sm_err("%s", "wcnt_exit module does not allow duplication.");
    return 0;
}

#include <iostream>

stockerrs::ERR_TYPE wcnt_exit::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::EXIT_BAR,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::EXIT_BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    connector* con = jwm.get_connectlist()->get_connector_by_input(this,
                                                    inputnames::IN_BAR);
    if (strcmp(con->get_output_module_name(), "off") == 0
     && exit_bar != 0)
    {
        sm_err("Input %s is turned off, and parameter %s is not zero. "
                            "wcnt would never exit if allowed to run!",
                    jwm.get_inputnames()->get_name(inputnames::IN_BAR),
                    jwm.get_paramnames()->get_name(paramnames::EXIT_BAR));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wcnt_exit::init()
{
    jwm.x_exit_bar = exit_bar;
    jwm.x_in_bar = in_bar;
}

void wcnt_exit::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::WCNTEXIT, paramnames::EXIT_BAR);
}

