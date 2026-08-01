#ifndef NOTETRAN_H
#define NOTETRAN_H

#include "synthmod.h"


// class notetran
//
// provides a non-standard translation of note names within a specified range
// to an output value within another speicifed range
//
// this module previously provided inputs, parameters, and outputs for
// handling both note_on and note_slide events from the sequencer, however,
// i have decided that was unessecary and that it would be preferable to either
// have a logic_trigger to combine the note_on and note_slide triggers, or, to
// setup two note_tran modules to handle the pair of triggers individually -
// the latter solution providing exactly the same functionality as the old
// single module.
//
// the benefits are:
//      * reduction of specialized parameters, inputs, and outputs
//      * the above now replaced with clearer standard versions
//      * reduction of duplicate code


class notetran: public synthmod::base
{
public:
    notetran(const char*);
    ~notetran();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    char const ** in_notename;
    double const * in_frequency;
    wcint_t const * in_detranspose;
    STATUS const* in_trig;
    // outputs
    double out_output;
    STATUS out_trig;
    STATUS out_not_trig;
    // params
    char* notename_lo;
    char* notename_hi;
    double min_out;
    double max_out;
    STATUS detranspose;
    double resptime;
    // working
    double lo_freq;
    double hi_freq;
    samp_t rt_samples;
    double respsize;
    void set_notename_lo(const char* nol);
    void set_notename_hi(const char* noh);
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
