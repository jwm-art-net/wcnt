#include "../include/notetran.h"
#include "../include/jwm_globals.h"
#include "../include/jwm_init.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

notetran::notetran(char const* uname) :
 synthmod(synthmodnames::NOTETRAN, uname, SM_DEFAULT),
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
    no_lo_notename = new char[jwm_init::note_array_size];
    no_hi_notename = new char[jwm_init::note_array_size];
    ns_lo_notename = new char[jwm_init::note_array_size];
    ns_hi_notename = new char[jwm_init::note_array_size];
    add_output(outputnames::OUT_NOVALUE);
    add_output(outputnames::OUT_NSVALUE);
    add_output(outputnames::OUT_NOTE_ON_TRIG);
    add_output(outputnames::OUT_NOT_NO_TRIG);
    add_output(outputnames::OUT_NOTE_SLIDE_TRIG);
    add_output(outputnames::OUT_NOT_NS_TRIG);
    add_input(inputnames::IN_NOTENAME);
    add_input(inputnames::IN_NOTE_ON_TRIG);
    add_input(inputnames::IN_NOTE_SLIDE_TRIG);
    add_input(inputnames::IN_DETRANSPOSE);
    create_params();
}

notetran::~notetran()
{
    delete [] no_lo_notename;
    delete [] no_hi_notename;
    delete [] ns_lo_notename;
    delete [] ns_hi_notename;
}

void const* notetran::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_NOVALUE:          return &out_no_value;
        case outputnames::OUT_NSVALUE:          return &out_ns_value;
        case outputnames::OUT_NOTE_ON_TRIG:     return &out_note_on_trig;
        case outputnames::OUT_NOT_NO_TRIG:      return &out_not_no_trig;
        case outputnames::OUT_NOTE_SLIDE_TRIG:return &out_note_slide_trig;
        case outputnames::OUT_NOT_NS_TRIG:      return &out_not_ns_trig;
        default: return 0;
    }
}

void const* notetran::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_NOTENAME:
        return in_notename = (char const**)o;
    case inputnames::IN_DETRANSPOSE:
        return in_detranspose = (short*)o;
    case inputnames::IN_NOTE_ON_TRIG:
        return in_note_on_trig = (STATUS*)o;
    case inputnames::IN_NOTE_SLIDE_TRIG:
        return in_note_slide_trig = (STATUS*)o;
    default:
        return 0;
    }
}

void const* notetran::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_NOTENAME:           return in_notename;
        case inputnames::IN_DETRANSPOSE:        return in_detranspose;
        case inputnames::IN_NOTE_ON_TRIG:       return in_note_on_trig;
        case inputnames::IN_NOTE_SLIDE_TRIG:    return in_note_slide_trig;
    default: return 0;
    }
}

bool notetran::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::NO_LONOTE:
        set_no_lo_notename((char*)data);
        return true;
    case paramnames::NO_HINOTE:
        set_no_hi_notename((char*)data);
        return true;
    case paramnames::NS_LONOTE:
        set_ns_lo_notename((char*)data);
        return true;
    case paramnames::NS_HINOTE:
        set_ns_hi_notename((char*)data);
        return true;
    case paramnames::MINNO_OUT:
        min_no_out = *(double*)data;
        return true;
    case paramnames::MAXNO_OUT:
        max_no_out = *(double*)data;
        return true;
    case paramnames::MINNS_OUT:
        min_ns_out = *(double*)data;
        return true;
    case paramnames::MAXNS_OUT:
        max_ns_out = *(double*)data;
        return true;
    case paramnames::DETRAN_NO:
        detran_no = *(STATUS*)data;
        return true;
    case paramnames::DETRAN_NS:
        detran_ns = *(STATUS*)data;
        return true;
    case paramnames::NO_RESPTIME:
        no_resptime = *(double*)data;
        return true;
    case paramnames::NS_RESPTIME:
        ns_resptime = *(double*)data;
        return true;
    default:
        return false;
    }
}

void const* notetran::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::NO_LONOTE:     return no_lo_notename;
        case paramnames::NO_HINOTE:     return no_hi_notename;
        case paramnames::NS_LONOTE:     return ns_lo_notename;
        case paramnames::NS_HINOTE:     return ns_hi_notename;
        case paramnames::MINNO_OUT:     return &min_no_out;
        case paramnames::MAXNO_OUT:     return &max_no_out;
        case paramnames::MINNS_OUT:     return &min_ns_out;
        case paramnames::MAXNS_OUT:     return &max_ns_out;
        case paramnames::DETRAN_NO:     return &detran_no;
        case paramnames::DETRAN_NS:     return &detran_ns;
        case paramnames::NO_RESPTIME:   return &no_resptime;
        case paramnames::NS_RESPTIME:   return &ns_resptime;
        default: return 0;
    }
}

stockerrs::ERR_TYPE notetran::validate()
{
    if (!check_notename(no_lo_notename)) {
        sm_err("%s %s",
                jwm.get_paramnames()->get_name(paramnames::NO_LONOTE),
                no_lo_notename);
        invalidate();
    }
    if (!check_notename(no_hi_notename)) {
        sm_err("%s %s",
                jwm.get_paramnames()->get_name(paramnames::NO_HINOTE),
                no_hi_notename);
        invalidate();
    }
    if (!check_notename(ns_lo_notename)) {
        sm_err("%s %s",
                jwm.get_paramnames()->get_name(paramnames::NS_LONOTE),
                ns_lo_notename);
        invalidate();
    }
    if (!check_notename(ns_hi_notename)) {
        sm_err("%s %s",
                jwm.get_paramnames()->get_name(paramnames::NS_HINOTE),
                ns_hi_notename);
        invalidate();
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::NO_RESPTIME,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s",
                jwm.get_paramnames()->get_name(paramnames::NO_RESPTIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::NS_RESPTIME,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s",
                jwm.get_paramnames()->get_name(paramnames::NS_RESPTIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void notetran::set_no_lo_notename(char const* nol)
{
    int n = strlen(nol);
    if (n > jwm_init::note_name_len)
        n = jwm_init::note_name_len;
    strncpy(no_lo_notename, nol, n);
    no_lo_notename[n] = '\0';
}

void notetran::set_no_hi_notename(char const* noh)
{
    int n = strlen(noh);
    if (n > jwm_init::note_name_len)
        n = jwm_init::note_name_len;
    strncpy(no_hi_notename, noh, n);
    no_hi_notename[n] = '\0';
}

void notetran::set_ns_lo_notename(char const* nsl)
{
    int n = strlen(nsl);
    if (n > jwm_init::note_name_len)
        n = jwm_init::note_name_len;
    strncpy(ns_lo_notename, nsl, n);
    ns_lo_notename[n] = '\0';
}

void notetran::set_ns_hi_notename(char const* nsh)
{
    int n = strlen(nsh);
    if (n > jwm_init::note_name_len)
        n = jwm_init::note_name_len;
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

void notetran::create_params()
{
    if (done_params())
        return;
    // inserted in order to be defined by user
    relate_param(paramnames::NO_LONOTE);
    relate_param(paramnames::NO_HINOTE);
    relate_param(paramnames::MINNO_OUT);
    relate_param(paramnames::MAXNO_OUT);
    relate_param(paramnames::DETRAN_NO);
    relate_param(paramnames::NO_RESPTIME);
    relate_param(paramnames::NS_LONOTE);
    relate_param(paramnames::NS_HINOTE);
    relate_param(paramnames::MINNS_OUT);
    relate_param(paramnames::MAXNS_OUT);
    relate_param(paramnames::DETRAN_NS);
    relate_param(paramnames::NS_RESPTIME);
}

