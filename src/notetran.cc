#ifndef TRIGGER_H
#include "../include/notetran.h"

notetran::notetran(char const* uname) :
 synthmod(synthmodnames::MOD_NOTETRAN, notetran_count, uname),
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
    no_lo_notename = new char[NOTE_ARRAY_SIZE];
    no_hi_notename = new char[NOTE_ARRAY_SIZE];
    ns_lo_notename = new char[NOTE_ARRAY_SIZE];
    ns_hi_notename = new char[NOTE_ARRAY_SIZE];
    get_outputlist()->add_output(this, outputnames::OUT_NOVALUE);
    get_outputlist()->add_output(this, outputnames::OUT_NSVALUE);
    get_outputlist()->add_output(this, outputnames::OUT_NOTE_ON_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOT_NO_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOTE_SLIDE_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOT_NS_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_NOTENAME);
    get_inputlist()->add_input(this, inputnames::IN_NOTE_ON_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_NOTE_SLIDE_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_DETRANSPOSE);
    notetran_count++;
    create_params();
}

notetran::~notetran()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
    delete [] no_lo_notename;
    delete [] no_hi_notename;
    delete [] ns_lo_notename;
    delete [] ns_hi_notename;
}

void const* notetran::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_NOVALUE:
        o = &out_no_value;
        break;
    case outputnames::OUT_NSVALUE:
        o = &out_ns_value;
        break;
    case outputnames::OUT_NOTE_ON_TRIG:
        o = &out_note_on_trig;
        break;
    case outputnames::OUT_NOT_NO_TRIG:
        o = &out_not_no_trig;
        break;
    case outputnames::OUT_NOTE_SLIDE_TRIG:
        o = &out_note_slide_trig;
        break;
    case outputnames::OUT_NOT_NS_TRIG:
        o = &out_not_ns_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* notetran::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_NOTENAME:
        i = in_notename = (char const**)o;
        break;
    case inputnames::IN_DETRANSPOSE:
        i = in_detranspose = (short*)o;
        break;
    case inputnames::IN_NOTE_ON_TRIG:
        i = in_note_on_trig = (STATUS*)o;
        break;
    case inputnames::IN_NOTE_SLIDE_TRIG:
        i = in_note_slide_trig = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool notetran::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_NO_LONOTE:
        set_no_lo_notename((char*)data);
        retv = true;
        break;
    case paramnames::PAR_NO_HINOTE:
        set_no_hi_notename((char*)data);
        retv = true;
        break;
    case paramnames::PAR_NS_LONOTE:
        set_ns_lo_notename((char*)data);
        retv = true;
        break;
    case paramnames::PAR_NS_HINOTE:
        set_ns_hi_notename((char*)data);
        retv = true;
        break;
    case paramnames::PAR_MINNO_OUT:
        set_min_no_out(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_MAXNO_OUT:
        set_max_no_out(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_MINNS_OUT:
        set_min_ns_out(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_MAXNS_OUT:
        set_max_ns_out(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_DETRAN_NO:
        set_detranspose_no(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_DETRAN_NS:
        set_detranspose_ns(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_NO_RESPTIME:
        set_no_response_time(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_NS_RESPTIME:
        set_ns_response_time(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* notetran::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_NO_LONOTE:
        return no_lo_notename;
    case paramnames::PAR_NO_HINOTE:
        return no_hi_notename;
    case paramnames::PAR_NS_LONOTE:
        return ns_lo_notename;
    case paramnames::PAR_NS_HINOTE:
        return ns_hi_notename;
    case paramnames::PAR_MINNO_OUT:
        return &min_no_out;
    case paramnames::PAR_MAXNO_OUT:
        return &max_no_out;
    case paramnames::PAR_MINNS_OUT:
        return &min_ns_out;
    case paramnames::PAR_MAXNS_OUT:
        return &max_ns_out;
    case paramnames::PAR_DETRAN_NO:
        return &detran_no;
    case paramnames::PAR_DETRAN_NS:
        return &detran_ns;
    case paramnames::PAR_NO_RESPTIME:
        return &no_resptime;
    case paramnames::PAR_NS_RESPTIME:
        return &ns_resptime;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE notetran::validate()
{
    if (!check_notename(no_lo_notename)) {
        *err_msg += get_paramnames()->get_name(paramnames::PAR_NO_LONOTE);
        *err_msg += " ";
        *err_msg += no_lo_notename;
        invalidate();
    }
    if (!check_notename(no_hi_notename)) {
        *err_msg += get_paramnames()->get_name(paramnames::PAR_NO_HINOTE);
        *err_msg += " ";
        *err_msg += no_hi_notename;
        invalidate();
    }
    if (!check_notename(ns_lo_notename)) {
        *err_msg += get_paramnames()->get_name(paramnames::PAR_NS_LONOTE);
        *err_msg += " ";
        *err_msg += ns_lo_notename;
        invalidate();
    }
    if (!check_notename(ns_hi_notename)) {
        *err_msg += get_paramnames()->get_name(paramnames::PAR_NS_HINOTE);
        *err_msg += " ";
        *err_msg += ns_hi_notename;
        invalidate();
    }
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_NO_RESPTIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_NO_RESPTIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_NS_RESPTIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_NS_RESPTIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void notetran::set_no_lo_notename(char const* nol)
{
    strncpy(no_lo_notename, nol, NOTE_NAME_LEN);
    no_lo_notename[NOTE_NAME_LEN] = '\0';
}

void notetran::set_no_hi_notename(char const* noh)
{
    strncpy(no_hi_notename, noh, NOTE_NAME_LEN);
    no_hi_notename[NOTE_NAME_LEN] = '\0';
}

void notetran::set_ns_lo_notename(char const* nsl)
{
    strncpy(ns_lo_notename, nsl, NOTE_NAME_LEN);
    ns_lo_notename[NOTE_NAME_LEN] = '\0';
}

void notetran::set_ns_hi_notename(char const* nsh)
{
    strncpy(ns_hi_notename, nsh, NOTE_NAME_LEN);
    ns_hi_notename[NOTE_NAME_LEN] = '\0';
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

int notetran::notetran_count = 0;

bool notetran::done_params = false;

void notetran::create_params()
{
    if (done_params == true)
        return;
    // inserted in order to be defined by user
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_NO_LONOTE);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_NO_HINOTE);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_MINNO_OUT);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_MAXNO_OUT);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_DETRAN_NO);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_NO_RESPTIME);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_NS_LONOTE);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_NS_HINOTE);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_MINNS_OUT);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_MAXNS_OUT);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_DETRAN_NS);
    get_paramlist()->add_param(
     synthmodnames::MOD_NOTETRAN, paramnames::PAR_NS_RESPTIME);
    done_params = true;
}

#endif
