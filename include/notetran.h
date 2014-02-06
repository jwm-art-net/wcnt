#ifndef NOTETRAN_H
#define NOTETRAN_H

#include "synthmod.h"

/*
// class notetran

// allows note names, in notedata, in riffs, in sequencer, to be put to
// a different use.

// As note on, and note_slide are two different events, but come
// from the same data, ie note_data in a riff, it provides two channels
// to set data for output at different times in the one riff.

// providing of course you follow the rules, which are quite simple:

// the note on stream arises when a new note is encountered, and no
// other note in that riff is currently playing.

// the note slide stream arises when a new note is encountered, and there
// is a note currently playing.

// note:

// when the sequencer encounters a note slide event, note_off is then
// triggered when the note which caused a note_slide, stops.
// (the original note's end is then ignored - unless it's still playing.)

// the note name is then interpreted, and, provided it is in the range
// specified in the notetran parameters, it is translated to a value
// within the output range.  if the note is outside of the input range,
// the output remains unchanged.

// the triggers triggering depends on note_on or note_slide causing the
// translation, and if the note name is within or out side of the
// corresponding note range.
*/

class notetran: public synthmod::base
{
public:
    notetran(const char*);
    ~notetran();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    // inputs
    char const ** in_notename;
    wcint_t const * in_detranspose;
    STATUS const* in_note_on_trig;
    STATUS const* in_note_slide_trig;
    // outputs
    double out_no_value;
    double out_ns_value;
    STATUS out_note_on_trig;
    STATUS out_not_no_trig;
    STATUS out_note_slide_trig;
    STATUS out_not_ns_trig;
    // params
    char* no_lo_notename;
    char* no_hi_notename;
    char* ns_lo_notename;
    char* ns_hi_notename;
    double min_no_out;
    double max_no_out;
    double min_ns_out;
    double max_ns_out;
    STATUS detran_no;
    STATUS detran_ns;
    double no_resptime;
    double ns_resptime;
    // working
    double nolo_freq;
    double nohi_freq;
    double nslo_freq;
    double nshi_freq;
    samp_t nort_samples;
    samp_t nsrt_samples;
    double no_respsize;
    double ns_respsize;
    void set_no_lo_notename(const char* nol);
    void set_no_hi_notename(const char* noh);
    void set_ns_lo_notename(const char* nsl);
    void set_ns_hi_notename(const char* nsh);
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
