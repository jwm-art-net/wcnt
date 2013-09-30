#include "../include/wcntexit.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/connectorlist.h"

wcnt_exit::wcnt_exit(const char* uname) :

 synthmod(module::WCNTEXIT, uname, SM_EMPTY_RUN | SM_UNGROUPABLE
                                                | SM_UNDUPLICABLE),
 in_bar(0), exit_bar(0)
{
    register_input(input::IN_BAR);
    init_first();
}

wcnt_exit::~wcnt_exit()
{
}

const void* wcnt_exit::set_in(input::TYPE it, const void* o)
{
    return (it == input::IN_BAR ? (in_bar = (short*)o) : 0);
}

const void* wcnt_exit::get_in(input::TYPE it) const
{
    return (it == input::IN_BAR ? in_bar : 0);
}

bool wcnt_exit::set_param(param::TYPE pt, const void* data)
{
    if (pt == param::EXIT_BAR) {
        exit_bar = *(short*)data;
        return true;
    }
    return false;
}

const void* wcnt_exit::get_param(param::TYPE pt) const
{
    return (pt == param::EXIT_BAR ? &exit_bar : 0);
}

synthmod* wcnt_exit::duplicate_module(const char* uname, DUP_IO dupio)
{
    (void)uname; (void)dupio; // stop unused param warns
    sm_err("%s", "wcnt_exit module does not allow duplication.");
    return 0;
}

errors::TYPE wcnt_exit::validate()
{
    if (!validate_param(param::EXIT_BAR, errors::NEGATIVE))
        return errors::NEGATIVE;

    connector* con = jwm.get_connectlist()->get_connector_by_input(this,
                                                            input::IN_BAR);
    if (strcmp(con->get_output_module_name(), "off") == 0
     && exit_bar != 0)
    {
        sm_err("Input %s is turned off, and parameter %s is not zero. "
               "wcnt would never exit if allowed to run!", 
                input::names::get(input::IN_BAR),
                    param::names::get(param::EXIT_BAR));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
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
    register_param(param::EXIT_BAR);
}

