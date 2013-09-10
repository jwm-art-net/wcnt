#ifndef SYNCCLOCK_H
#define SYNCCLOCK_H

#include "conversions.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

// for mandatory conversions..
#include "timemap.h"

class sync_clock : public synthmod
{
public:
    sync_clock(char const*);
    ~sync_clock();
    // set inputs
    void set_input_bpm(const double* bpm){in_bpm = bpm;}
    void set_input_pos_step_size(const double* bs){in_pos_stepsz = bs;}
    void set_input_beats_per_bar(const short* bpb){in_beats_per_bar=bpb;}
    void set_input_beat_value(const short* bv){ in_beat_value = bv;}
    void set_input_phase_trig(const STATUS* pt){ in_phase_trig = pt;}
    void set_input_freq_mod1(const double* fm){ in_freq_mod1 = fm; }
    void set_input_freq_mod2(const double* fm){ in_freq_mod2 = fm; }
    // get outputs
    const STATUS* get_output_phase_trig(){ return &out_phase_trig; }
    const double* get_output_deg_size(){ return &out_deg_size; }
    // set parameters
    void set_freq_mod1size(double fms){ freq_mod1size = fms; }
    void set_freq_mod2size(double fms){ freq_mod2size = fms; }
    void set_quarter_value(short qv) { quarter_val = qv;}
    void set_note_length(double nl) { note_length = nl;}
    void set_snap_to(double sn) { snap_to = sn;}
    double get_freq_mod1size(){ return freq_mod1size; }
    double get_freq_mod2size(){ return freq_mod2size; }
    short get_quarter_value(){ return quarter_val;}
    double get_note_length(){ return note_length;}
    double get_snap_to(){ return snap_to;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    // inputs
    const double* in_bpm;
    const double* in_pos_stepsz;
    const short* in_beats_per_bar;
    const short* in_beat_value;
    const STATUS* in_phase_trig;
    const double* in_freq_mod1;
    const double* in_freq_mod2;
    // outputs
    STATUS out_phase_trig;
    double out_deg_size;
    // parameters
    double freq_mod1size;
    double freq_mod2size;
    short quarter_val;
    double note_length;
    double snap_to;
    // working
    double premod_nlen;
    double mod1size;
    double mod2size;
    double posconv;
    double phaselen;
    double beatlen;
    double phasepos;
    double snapto;
    static int sync_clock_count;
    void create_params();
    static bool done_params;
};

#endif
