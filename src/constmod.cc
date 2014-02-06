#include "../include/constmod.h"

constmod::constmod(const char* uname) :
 synthmod::base(synthmod::CONSTMOD, uname, SM_EMPTY_RUN | SM_HAS_OUT_OUTPUT),
 output(0)
{
    register_output(output::OUT_OUTPUT);
}

void constmod::register_ui()
{
    register_param(param::VALUE);
}

ui::moditem_list* constmod::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

constmod::~constmod()
{
}

const void* constmod::get_out(int ot) const
{
    switch(ot) 
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

bool constmod::set_param(int pt, const void* data)
{
    switch(pt) {
        case param::VALUE:
            output = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* constmod::get_param(int pt) const
{
    switch(pt)
    {
        // parameter /is/ output.
        case param::VALUE: return &output;
        default: return 0;
    }
}

