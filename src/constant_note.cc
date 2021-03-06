#include "../include/constant_note.h"
#include "../include/globals.h"
#include "../include/conversions.h"

constant_note::constant_note(const char* uname) :
 synthmod::base(synthmod::CONSTANT_NOTE, uname, SM_EMPTY_RUN),
 out_freq(0.0), out_phase_step(0.0)
{
    register_output(output::OUT_NOTENAME);
    register_output(output::OUT_FREQ);
    register_output(output::OUT_PHASE_STEP);
    note = new char[wcnt::note_array_size];
}

void constant_note::register_ui()
{
    register_param(param::NAME);
}

ui::moditem_list* constant_note::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

constant_note::~constant_note()
{
    if (note)
        delete [] note;
}

const void* constant_note::get_out(output::TYPE ot) const
{
    switch(ot) 
    {
        case output::OUT_NOTENAME:   return &note;
        case output::OUT_FREQ:       return &out_freq;
        case output::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

bool constant_note::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
        case param::NAME:
            set_note((const char*)data);
            return true;
        default:
            return false;
    }
}

const void* constant_note::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::NAME: return note;
        default: return 0;
    }
}

errors::TYPE constant_note::validate()
{
    if (!check_notename(note)){
        sm_err("%s", param::names::get(param::NAME));
        invalidate();
        return errors::NOTENAME;
    }
    return errors::NO_ERROR;
}

void constant_note::init()
{
    out_freq = note_to_freq(note);
    out_phase_step = freq_to_step(out_freq);
}

void constant_note::set_note(const char* n)
{
    strncpy(note, n, wcnt::note_name_len);
    note[wcnt::note_name_len] = '\0';
}

