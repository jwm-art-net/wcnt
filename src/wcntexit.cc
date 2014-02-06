#include "../include/wcntexit.h"
#include "../include/globals.h"
#include "../include/connectorlist.h"

wcnt_exit::wcnt_exit(const char* uname) :

 synthmod::base(synthmod::WCNTEXIT, uname, SM_EMPTY_RUN | SM_UNGROUPABLE
                                                | SM_UNDUPLICABLE),
 in_bar(0), exit_bar(0)
{
}

void wcnt_exit::register_ui()
{
    register_input(input::IN_BAR);
    register_param(param::EXIT_BAR);
}

ui::moditem_list* wcnt_exit::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

wcnt_exit::~wcnt_exit()
{
}

const void* wcnt_exit::set_in(int it, const void* o)
{
    return (it == input::IN_BAR ? (in_bar = (wcint_t*)o) : 0);
}

const void* wcnt_exit::get_in(int it) const
{
    return (it == input::IN_BAR ? in_bar : 0);
}

bool wcnt_exit::set_param(int pt, const void* data)
{
    if (pt == param::EXIT_BAR) {
        exit_bar = *(wcint_t*)data;
        return true;
    }
    return false;
}

const void* wcnt_exit::get_param(int pt) const
{
    return (pt == param::EXIT_BAR ? &exit_bar : 0);
}

synthmod::base* wcnt_exit::duplicate_module(const char*, DUP_IO)
{
    sm_err("%s", "wcnt_exit module does not allow duplication.");
    return 0;
}

errors::TYPE wcnt_exit::validate()
{
    if (!validate_param(param::EXIT_BAR, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    connector* con = wcnt::get_connectlist()
                   ->get_connector_by_input(this, input::IN_BAR);

    #ifdef DEBUG
    if (!con) {
        sm_err("%s no connector found for input '%s'.",
                errors::stock::major, input::names::get(input::IN_BAR));
        invalidate();
        return errors::ERROR;
    }
    #endif

    if (strcmp(con->get_output_module_name(), "off") == 0 && exit_bar != 0)
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
    wcnt::jwm.x_exit_bar = exit_bar;
    wcnt::jwm.x_in_bar = in_bar;
}
