#include "../include/wcntexit.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/connectorlist.h"

wcnt_exit::wcnt_exit(const char* uname) :

 synthmod(
    synthmodnames::WCNTEXIT,
    uname,
    SM_EMPTY_RUN | SM_UNGROUPABLE | SM_UNDUPLICABLE),

 in_bar(0), exit_bar(0)
{
    register_input(inputnames::IN_BAR);
    init_first();
}

wcnt_exit::~wcnt_exit()
{
}

const void* wcnt_exit::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it)
    {
        case inputnames::IN_BAR: return in_bar = (short*)o;
        default: return 0;
    }
}

const void* wcnt_exit::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_BAR: return in_bar;
        default: return 0;
    }
}

bool wcnt_exit::set_param(paramnames::PAR_TYPE pt, const void* data)
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

const void* wcnt_exit::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::EXIT_BAR: return &exit_bar;
        default: return 0;
    }
}

synthmod* wcnt_exit::duplicate_module(const char* uname, DUP_IO dupio)
{
    (void)uname; (void)dupio; // stop unused param warns
    sm_err("%s", "wcnt_exit module does not allow duplication.");
    return 0;
}

stockerrs::ERR_TYPE wcnt_exit::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::EXIT_BAR,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", paramnames::get_name(paramnames::EXIT_BAR));
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
                    inputnames::get_name(inputnames::IN_BAR),
                    paramnames::get_name(paramnames::EXIT_BAR));
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

void wcnt_exit::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::EXIT_BAR);
}

