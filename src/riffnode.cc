#ifndef RIFFNODE_H
#include "../include/riffnode.h"

riff_node::riff_node() :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(0), riff_source(0),
 transpose(0), startlen(0), endlen(0)

{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

riff_node::riff_node(riffdata* rd, short barpos) :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(barpos), riff_source(rd)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

riff_node::~riff_node()
{
}

#ifndef BARE_MODULES

bool riff_node::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_RIFFNAME:
        set_riff_source((riffdata*)data);// pass pointer
        retv = true;
        break;
    case dparamnames::DPAR_BAR:
        set_start_bar(*(short*)data);
        retv = true;
        break;
    case dparamnames::DPAR_TRANSPOSE:
        set_transpose(*(short*)data);
        retv = true;
        break;
    case dparamnames::DPAR_STARTLEN:
        set_start_length(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_ENDLEN:
        set_end_length(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* riff_node::get_dparam(dparamnames::DPAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case dparamnames::DPAR_RIFFNAME:
        retv = riff_source;
        break;
    case dparamnames::DPAR_BAR:
        retv = &start_bar;
        break;
    case dparamnames::DPAR_TRANSPOSE:
        retv = &transpose;
        break;
    case dparamnames::DPAR_STARTLEN:
        retv = &startlen;
        break;
    case dparamnames::DPAR_ENDLEN:
        retv = &endlen;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE riff_node::validate()
{
    if (!get_dparlist()->validate(
        this, dparamnames::DPAR_BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_dparnames()->get_name(dparamnames::DPAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool riff_node::done_dparams = false;

void riff_node::create_dparams()
{
    if (done_dparams == true) return;
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, dparamnames::DPAR_RIFFNAME);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, dparamnames::DPAR_BAR);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, dparamnames::DPAR_TRANSPOSE);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, dparamnames::DPAR_STARTLEN);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, dparamnames::DPAR_ENDLEN);
    done_dparams = true;
}

#endif // BARE_MODULES
#endif
