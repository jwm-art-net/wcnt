#include "../include/notetran.h"
#include "../include/globals.h"
#include "../include/conversions.h"

notetran::notetran(const char* uname) :
 synthmod::base(synthmod::NOTETRAN, uname, SM_DEFAULT),
 in_notename(0), in_frequency(0), in_detranspose(0), in_trig(0),
 out_output(0), out_trig(OFF), out_not_trig(OFF),
 notename_lo(0), notename_hi(0), min_out(0), max_out(0),
 detranspose(OFF), resptime(0),
 lo_freq(0), hi_freq(0), rt_samples(0), respsize(0)
{
    notename_lo = new char[wcnt::note_array_size];
    notename_hi = new char[wcnt::note_array_size];
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_TRIG);
    register_output(output::OUT_NOT_TRIG);
}

void notetran::register_ui()
{
    register_input(input::IN_NOTENAME);
    register_input(input::IN_FREQ)->set_flags(ui::UI_OPTIONAL)->add_descr("only required if riff data contains notes set with a frequency.");
    register_input(input::IN_TRIG);
    register_param(param::LONOTE);
    register_param(param::HINOTE);
    register_param(param::MIN_OUT);
    register_param(param::MAX_OUT);
    register_param(param::RESPONSE_TIME)->set_flags(ui::UI_OPTIONAL);
    register_input(input::IN_DETRANSPOSE)->set_flags(ui::UI_OPTIONAL);
}

ui::moditem_list* notetran::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

notetran::~notetran()
{
    delete [] notename_lo;
    delete [] notename_hi;
}

const void* notetran::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT:    return &out_output;
        case output::OUT_TRIG:      return &out_trig;
        case output::OUT_NOT_TRIG:  return &out_not_trig;
        default: return 0;
    }
}

const void* notetran::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_NOTENAME:
        return in_notename = (const char**)o;
    case input::IN_FREQ:
        return in_frequency = (const double*)o;
    case input::IN_DETRANSPOSE:
        return in_detranspose = (wcint_t*)o;
    case input::IN_TRIG:
        return in_trig = (STATUS*)o;
    default:
        return 0;
    }
}

const void* notetran::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_NOTENAME:    return in_notename;
        case input::IN_FREQ:        return in_frequency;
        case input::IN_DETRANSPOSE: return in_detranspose;
        case input::IN_TRIG:        return in_trig;
    default: return 0;
    }
}

bool notetran::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::LONOTE:
        set_notename_lo((const char*)data);
        return true;
    case param::HINOTE:
        set_notename_hi((const char*)data);
        return true;
    case param::MIN_OUT:
        min_out = *(double*)data;
        return true;
    case param::MAX_OUT:
        max_out = *(double*)data;
        return true;
    case param::RESPONSE_TIME:
        resptime = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* notetran::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::LONOTE:     return notename_lo;
        case param::HINOTE:     return notename_hi;
        case param::MIN_OUT:    return &min_out;
        case param::MAX_OUT:    return &max_out;
        case param::RESPONSE_TIME:  return &resptime;
        default: return 0;
    }
}

errors::TYPE notetran::validate()
{
    if (!check_notename(notename_lo)) {
        sm_err("%s %s", param::names::get(param::LONOTE), notename_lo);
        invalidate();
    }
    if (!check_notename(notename_hi)) {
        sm_err("%s %s", param::names::get(param::HINOTE), notename_hi);
        invalidate();
    }
    if (!validate_param(param::RESPONSE_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

void notetran::set_notename_lo(const char* nn)
{
    strncpy(notename_lo, nn, wcnt::note_name_len);
    notename_lo[wcnt::note_name_len] = '\0';
}

void notetran::set_notename_hi(const char* nn)
{
    strncpy(notename_hi, nn, wcnt::note_name_len);
    notename_hi[wcnt::note_name_len] = '\0';
}

void notetran::init()
{
    lo_freq = note_to_freq(notename_lo);
    hi_freq = note_to_freq(notename_hi);
    respsize = (double) 1 / ms_to_samples(resptime);
}

void notetran::run()
{
    if (*in_trig == ON) {
        double infreq;
        if (strcmp(*in_notename, " freq") == 0)
            infreq = transpose_frequency(*in_frequency, -*in_detranspose);
        else
            infreq = note_to_freq(*in_notename, 0, -*in_detranspose);
        if (infreq < lo_freq || infreq > hi_freq)
            out_not_trig = ON;
        else {
            out_trig = ON;
            double targ = min_out + (max_out - min_out)
                            * ((infreq - lo_freq) / (hi_freq - lo_freq));
            if (resptime > 0) {
                rt_samples = ms_to_samples(resptime);
                respsize = (targ - out_output) / rt_samples;
            }
            else
                out_output = targ;
        }
    }
    else {
        if (out_not_trig == ON) out_not_trig = OFF;
        if (out_trig == ON) out_trig = OFF;
        if (rt_samples > 0) {
            rt_samples--;
            out_output += respsize;
        }
    }
}

