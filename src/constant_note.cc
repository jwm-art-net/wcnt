#ifndef CONSTANT_NOTE_H
#include "../include/constant_note.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

constant_note::constant_note(char const* uname) :
 synthmod(synthmodnames::CONSTANT_NOTE, uname),
 out_freq(0.0), out_phase_step(0.0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_NOTENAME);
    jwm.get_outputlist().add_output(this, outputnames::OUT_FREQ);
    jwm.get_outputlist().add_output(this, outputnames::OUT_PHASE_STEP);
    create_params();
}

constant_note::~constant_note()
{
    jwm.get_outputlist().delete_module_outputs(this);
}

void const* constant_note::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot) 
    {
        case outputnames::OUT_NOTENAME:   return note;
        case outputnames::OUT_FREQ:       return &out_freq;
        case outputnames::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

bool constant_note::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt) {
        case paramnames::NAME:
            set_note((const char*)data);
            return true;
        default:
            return false;
    }
}

void const* constant_note::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::NAME: return note;
        default: return 0;
    }
}

stockerrs::ERR_TYPE constant_note::validate()
{
    if (!check_notename(note)){
        *err_msg = jwm.get_paramnames().get_name(paramnames::NAME);
        invalidate();
        return stockerrs::ERR_NOTENAME;
    }
    return stockerrs::ERR_NO_ERROR;
}

void constant_note::init()
{
    out_freq = note_to_freq(note);
    out_phase_step = freq_to_step(out_freq);
}

void constant_note::set_note(const char* n)
{
    strncpy(note, n, jwm_init::note_name_len);
    note[jwm_init::note_name_len] = '\0';
}

bool constant_note::done_params = false;

void constant_note::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
        synthmodnames::CONSTANT_NOTE, paramnames::NAME);
    done_params = true;
}

#endif
