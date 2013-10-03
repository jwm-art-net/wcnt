#include "../include/inspect.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

#include <iostream>

inspect::inspect(const char* uname) :
 synthmod(module::INSPECT, uname, SM_DEFAULT),
 in_df(0),  in_df_trig(0),
 in_si(0),  in_si_trig(0),
 in_ul(0),  in_ul_trig(0),
 in_st(0),  in_st_trig(0),
 in_str(0), in_str_trig(0),
 df_msg(0), si_msg(0), ul_msg(0), st_msg(0), str_msg(0)
{
    register_input(input::IN_INSP_DF);
    register_input(input::IN_INSP_DF_TRIG);
    register_input(input::IN_INSP_SI);
    register_input(input::IN_INSP_SI_TRIG);
    register_input(input::IN_INSP_UL);
    register_input(input::IN_INSP_UL_TRIG);
    register_input(input::IN_INSP_ST);
    register_input(input::IN_INSP_ST_TRIG);
    register_input(input::IN_INSP_STR);
    register_input(input::IN_INSP_STR_TRIG);
    init_first();
}

inspect::~inspect()
{
    if (df_msg)
        delete [] df_msg;
    if (si_msg)
        delete [] si_msg;
    if (ul_msg)
        delete [] ul_msg;
    if (st_msg)
        delete [] st_msg;
    if (str_msg)
        delete [] str_msg;
}

void inspect::set_message(const char* msg_from, char** msg_to)
{
    if (*msg_to) {
        delete [] *msg_to;
        *msg_to = 0;
    }
    *msg_to = new char[strlen(msg_from) + 1];
    strcpy(*msg_to, msg_from);
}

const void* inspect::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_INSP_DF:        return in_df = (const double*)o;
    case input::IN_INSP_DF_TRIG:   return in_df_trig = (const STATUS*)o;
    case input::IN_INSP_SI:        return in_si = (const wcint_t*)o;
    case input::IN_INSP_SI_TRIG:   return in_si_trig = (const STATUS*)o;
    case input::IN_INSP_UL:        return in_ul = (const samp_t*)o;
    case input::IN_INSP_UL_TRIG:   return in_ul_trig = (const STATUS*)o;
    case input::IN_INSP_ST:        return in_st = (STATUS const*)o;
    case input::IN_INSP_ST_TRIG:   return in_st_trig = (const STATUS*)o;
    case input::IN_INSP_STR:       return in_str = (const char**) o;
    case input::IN_INSP_STR_TRIG:  return in_str_trig = (const STATUS*) o;
    default: return 0;
    }
}

const void* inspect::get_in(input::TYPE it) const
{
    switch(it)
    {
    case input::IN_INSP_DF:        return in_df;
    case input::IN_INSP_DF_TRIG:   return in_df_trig;
    case input::IN_INSP_SI:        return in_si;
    case input::IN_INSP_SI_TRIG:   return in_si_trig;
    case input::IN_INSP_UL:        return in_ul;
    case input::IN_INSP_UL_TRIG:   return in_ul_trig;
    case input::IN_INSP_ST:        return in_st;
    case input::IN_INSP_ST_TRIG:   return in_st_trig;
    case input::IN_INSP_STR:       return in_str;
    case input::IN_INSP_STR_TRIG:  return in_str_trig;
    default: return 0;
    }
}

void inspect::run()
{
    if (*in_df_trig == ON) {
        std::cout << "\n" << df_msg << " " << *in_df << " ";
    }
    if (*in_si_trig == ON) {
        std::cout << "\n" << si_msg << " " << *in_si << " ";
    }
    if (*in_ul_trig == ON) {
        std::cout << "\n" << ul_msg << " " << *in_ul << " ";
    }
    if (*in_st_trig == ON) {
        std::cout << "\n" << st_msg << " " 
            << (*in_st == ON ? "on" : "off") << " ";
    }
    if (*in_str_trig == ON) {
        std::cout << "\n" << str_msg << " " << *in_str << " ";
    }
}

bool inspect::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::INSPECT_DF_MSG:
            set_message((const char*)data, &df_msg);
            return true;
        case param::INSPECT_SI_MSG:
            set_message((const char*)data, &si_msg);
            return true;
        case param::INSPECT_UL_MSG:
            set_message((const char*)data, &ul_msg);
            return true;
        case param::INSPECT_ST_MSG:
            set_message((const char*)data, &st_msg);
            return true;
        case param::INSPECT_STR_MSG:
            set_message((const char*)data, &str_msg);
            return true;
        default:
            return false;
    }
}

const void* inspect::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::INSPECT_DF_MSG:  return df_msg;
        case param::INSPECT_SI_MSG:  return si_msg;
        case param::INSPECT_UL_MSG:  return ul_msg;
        case param::INSPECT_ST_MSG:  return st_msg;
        case param::INSPECT_STR_MSG: return str_msg;
        default: return 0;
    }
}



void inspect::init_first()
{
    if (done_first())
        return;
    register_param(param::INSPECT_DF_MSG);
    register_param(param::INSPECT_SI_MSG);
    register_param(param::INSPECT_UL_MSG);
    register_param(param::INSPECT_ST_MSG);
    register_param(param::INSPECT_STR_MSG);
}
