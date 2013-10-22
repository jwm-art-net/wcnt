#include "../include/notetran.h"
#include "../include/globals.h"
#include "../include/conversions.h"

notetran::notetran(const char* uname) :
 synthmod::base(synthmod::NOTETRAN, uname, SM_DEFAULT),
 in_notename(0), in_detranspose(0), in_note_on_trig(0),
 in_note_slide_trig(0), out_no_value(0), out_ns_value(0),
 out_note_on_trig(OFF), out_not_no_trig(OFF), out_note_slide_trig(OFF),
 out_not_ns_trig(OFF), no_lo_notename(0), no_hi_notename(0),
 ns_lo_notename(0), ns_hi_notename(0), min_no_out(0), max_no_out(0),
 min_ns_out(0), max_ns_out(0), detran_no(OFF), detran_ns(OFF),
 no_resptime(0), ns_resptime(0),	nolo_freq(0), nohi_freq(0),
 nslo_freq(0),nshi_freq(0), nort_samples(0), nsrt_samples(0),
 no_respsize(0), ns_respsize(0)
{
    no_lo_notename = new char[wcnt::note_array_size];
    no_hi_notename = new char[wcnt::note_array_size];
    ns_lo_notename = new char[wcnt::note_array_size];
    ns_hi_notename = new char[wcnt::note_array_size];
    register_output(output::OUT_NOVALUE);
    register_output(output::OUT_NSVALUE);
    register_output(output::OUT_NOTE_ON_TRIG);
    register_output(output::OUT_NOT_NO_TRIG);
    register_output(output::OUT_NOTE_SLIDE_TRIG);
    register_output(output::OUT_NOT_NS_TRIG);
}

void notetran::register_ui()
{
    register_input(input::IN_NOTENAME);
    register_input(input::IN_NOTE_ON_TRIG);
    register_param(param::NO_LONOTE);
    register_param(param::NO_HINOTE);
    register_param(param::MINNO_OUT);
    register_param(param::MAXNO_OUT);
    register_param(param::DETRAN_NO);
    register_param(param::NO_RESPTIME);
    register_input(input::IN_NOTE_SLIDE_TRIG);
    register_param(param::NS_LONOTE);
    register_param(param::NS_HINOTE);
    register_param(param::MINNS_OUT);
    register_param(param::MAXNS_OUT);
    register_param(param::DETRAN_NS);
    register_param(param::NS_RESPTIME);
    register_input(input::IN_DETRANSPOSE);
}

ui::moditem_list* notetran::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

notetran::~notetran()
{
    delete [] no_lo_notename;
    delete [] no_hi_notename;
    delete [] ns_lo_notename;
    delete [] ns_hi_notename;
}

const void* notetran::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_NOVALUE:          return &out_no_value;
        case output::OUT_NSVALUE:          return &out_ns_value;
        case output::OUT_NOTE_ON_TRIG:     return &out_note_on_trig;
        case output::OUT_NOT_NO_TRIG:      return &out_not_no_trig;
        case output::OUT_NOTE_SLIDE_TRIG:return &out_note_slide_trig;
        case output::OUT_NOT_NS_TRIG:      return &out_not_ns_trig;
        default: return 0;
    }
}

const void* notetran::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_NOTENAME:
        return in_notename = (const char**)o;
    case input::IN_DETRANSPOSE:
        return in_detranspose = (wcint_t*)o;
    case input::IN_NOTE_ON_TRIG:
        return in_note_on_trig = (STATUS*)o;
    case input::IN_NOTE_SLIDE_TRIG:
        return in_note_slide_trig = (STATUS*)o;
    default:
        return 0;
    }
}

const void* notetran::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_NOTENAME:           return in_notename;
        case input::IN_DETRANSPOSE:        return in_detranspose;
        case input::IN_NOTE_ON_TRIG:       return in_note_on_trig;
        case input::IN_NOTE_SLIDE_TRIG:    return in_note_slide_trig;
    default: return 0;
    }
}

bool notetran::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::NO_LONOTE:
        set_no_lo_notename((const char*)data);
        return true;
    case param::NO_HINOTE:
        set_no_hi_notename((const char*)data);
        return true;
    case param::NS_LONOTE:
        set_ns_lo_notename((const char*)data);
        return true;
    case param::NS_HINOTE:
        set_ns_hi_notename((const char*)data);
        return true;
    case param::MINNO_OUT:
        min_no_out = *(double*)data;
        return true;
    case param::MAXNO_OUT:
        max_no_out = *(double*)data;
        return true;
    case param::MINNS_OUT:
        min_ns_out = *(double*)data;
        return true;
    case param::MAXNS_OUT:
        max_ns_out = *(double*)data;
        return true;
    case param::DETRAN_NO:
        detran_no = *(STATUS*)data;
        return true;
    case param::DETRAN_NS:
        detran_ns = *(STATUS*)data;
        return true;
    case param::NO_RESPTIME:
        no_resptime = *(double*)data;
        return true;
    case param::NS_RESPTIME:
        ns_resptime = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* notetran::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::NO_LONOTE:     return no_lo_notename;
        case param::NO_HINOTE:     return no_hi_notename;
        case param::NS_LONOTE:     return ns_lo_notename;
        case param::NS_HINOTE:     return ns_hi_notename;
        case param::MINNO_OUT:     return &min_no_out;
        case param::MAXNO_OUT:     return &max_no_out;
        case param::MINNS_OUT:     return &min_ns_out;
        case param::MAXNS_OUT:     return &max_ns_out;
        case param::DETRAN_NO:     return &detran_no;
        case param::DETRAN_NS:     return &detran_ns;
        case param::NO_RESPTIME:   return &no_resptime;
        case param::NS_RESPTIME:   return &ns_resptime;
        default: return 0;
    }
}

errors::TYPE notetran::validate()
{
    if (!check_notename(no_lo_notename)) {
        sm_err("%s %s",
                param::names::get(param::NO_LONOTE),
                no_lo_notename);
        invalidate();
    }
    if (!check_notename(no_hi_notename)) {
        sm_err("%s %s",
                param::names::get(param::NO_HINOTE),
                no_hi_notename);
        invalidate();
    }
    if (!check_notename(ns_lo_notename)) {
        sm_err("%s %s",
                param::names::get(param::NS_LONOTE),
                ns_lo_notename);
        invalidate();
    }
    if (!check_notename(ns_hi_notename)) {
        sm_err("%s %s",
                param::names::get(param::NS_HINOTE),
                ns_hi_notename);
        invalidate();
    }
    if (!validate_param(param::NO_RESPTIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::NS_RESPTIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

void notetran::set_no_lo_notename(const char* nol)
{
    int n = strlen(nol);
    if (n > wcnt::note_name_len)
        n = wcnt::note_name_len;
    strncpy(no_lo_notename, nol, n);
    no_lo_notename[n] = '\0';
}

void notetran::set_no_hi_notename(const char* noh)
{
    int n = strlen(noh);
    if (n > wcnt::note_name_len)
        n = wcnt::note_name_len;
    strncpy(no_hi_notename, noh, n);
    no_hi_notename[n] = '\0';
}

void notetran::set_ns_lo_notename(const char* nsl)
{
    int n = strlen(nsl);
    if (n > wcnt::note_name_len)
        n = wcnt::note_name_len;
    strncpy(ns_lo_notename, nsl, n);
    ns_lo_notename[n] = '\0';
}

void notetran::set_ns_hi_notename(const char* nsh)
{
    int n = strlen(nsh);
    if (n > wcnt::note_name_len)
        n = wcnt::note_name_len;
    strncpy(ns_hi_notename, nsh, n);
    ns_hi_notename[n] = '\0';
}

void notetran::init()
{
    nolo_freq = note_to_freq(no_lo_notename);
    nohi_freq = note_to_freq(no_hi_notename);
    nslo_freq = note_to_freq(ns_lo_notename);
    nshi_freq = note_to_freq(ns_hi_notename);
    no_respsize = (double) 1 / ms_to_samples(no_resptime);
    ns_respsize = (double) 1 / ms_to_samples(ns_resptime);
}

void notetran::run()
{
    if (*in_note_on_trig == ON) {
        double infreq_no = note_to_freq(*in_notename, 0,
         (detran_no == OFF) ? 0 : -*in_detranspose);
        if (infreq_no < nolo_freq || infreq_no > nohi_freq)
            out_not_no_trig = ON;
        else {
            out_note_on_trig = ON;
            double targ = min_no_out + (max_no_out - min_no_out)
              * ((infreq_no - nolo_freq) / (nohi_freq - nolo_freq));
            if (no_resptime > 0) {
                nort_samples = ms_to_samples(no_resptime);
                no_respsize = (targ - out_no_value) / nort_samples;
            } else out_no_value = targ;
        }
    }
    else {
        if (out_not_no_trig == ON) out_not_no_trig = OFF;
        if (out_note_on_trig == ON) out_note_on_trig = OFF;
        if (nort_samples > 0) {
            nort_samples--;
            out_no_value += no_respsize;
        }
    }
    if (*in_note_slide_trig == ON) {
        double infreq_ns = note_to_freq(*in_notename, 0,
         (detran_ns == OFF) ? 0 : -*in_detranspose);
        if (infreq_ns < nslo_freq || infreq_ns > nshi_freq)
            out_not_ns_trig = ON;
        else {
            out_note_slide_trig = ON;
            double targ = min_ns_out + (max_ns_out - min_ns_out)
             * ((infreq_ns - nslo_freq) / (nshi_freq - nslo_freq));
            if (ns_resptime > 0) {
                nsrt_samples = ms_to_samples(ns_resptime);
                ns_respsize = (targ - out_ns_value) / nsrt_samples;
            } else out_ns_value = targ;
        }
    }
    else {
        if (out_not_ns_trig == ON) out_not_ns_trig = OFF;
        if (out_note_slide_trig == ON) out_note_slide_trig = OFF;
        if (nsrt_samples > 0) {
            nsrt_samples--;
            out_ns_value += ns_respsize;
        }
    }
}

