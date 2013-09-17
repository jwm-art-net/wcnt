#include "../include/constant_note.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

constant_note::constant_note(char const* uname) :
 synthmod(synthmodnames::CONSTANT_NOTE, uname, SM_EMPTY_RUN),
 out_freq(0.0), out_phase_step(0.0)
{
    add_output(outputnames::OUT_NOTENAME);
    add_output(outputnames::OUT_FREQ);
    add_output(outputnames::OUT_PHASE_STEP);
    note = new char[jwm_init::note_array_size];
    create_params();
}

constant_note::~constant_note()
{
    if (note)
        delete [] note;
}

void const* constant_note::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot) 
    {
        case outputnames::OUT_NOTENAME:   return &note;
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
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::NAME));
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

void constant_note::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::NAME);
}

