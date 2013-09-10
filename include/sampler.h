#ifndef SAMPLER_H
#define SAMPLER_H

#include "samplercommon.h"
#include "synthmod.h"

/* removed all sorts of guff I'd written in 70 lines of comment */

class wavfilein;
class st_data;

class sampler : public synthmod
{
public:
    sampler(char const*);
    ~sampler();
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    // inputs
    const STATUS* in_play_trig;
    const STATUS* in_stop_trig;
    const double* in_start_pos_mod;
    const double* in_phase_step;
    // outputs
    double out_left;
    double out_right;
    STATUS out_loop_trig;
    STATUS play_state;
    // params
    wavfilein* wavfile;
    PLAY_DIR play_dir;
    PLAY_MODE play_mode;
    JUMP_DIR jump_mode;
    unsigned long min_start_pos;
    unsigned long max_start_pos;
    unsigned long loop_begin;
    unsigned long loop_end;
    STATUS loop_is_offset;
    LOOP_MODE loop_mode;
    short loop_bi_offset;
    short anti_clip_size;
    STATUS ac_each_end;
    short search_range;
    double phase_step_amount;
    // working
    double root_phase_step;
    PLAY_DIR playdir;
    PLAY_DIR acplaydir;
    bool loop_yet;
    double* mono_buffer;
    double* ac_m_buf;
    st_data* st_buffer;
    st_data* ac_st_buf;
    unsigned long buffer_start_pos;
    int buff_pos;
    unsigned long wavstart;
    unsigned long wavlength;
    unsigned long wavstbi;
    unsigned long wavlenbi;
    double cur_pos;
    double ac_cpstep;
    double oldcpstp;
    double cp_step;
    double cp_ratio;
    double sr_ratio; //ratio between input and output samplerates
    double bp_midpoint;
    unsigned long start_pos;
    unsigned long startpos_span;
    unsigned long loopstart;
    unsigned long loopfinish;
    bool loop_fits_in_buffer;
    bool loop_loaded;
    STATUS do_ac;
    double ac_cur_pos;
    double ac_midpoint;
    int ac_buf_pos;
    unsigned long ac_buf_start_pos;
    double ac_step;
    double ac_size;
    double ac_out_left;
    double ac_out_right;
    short ch;
    unsigned long sampletot;
    // private helper functions:
    inline void fill_buffer(unsigned long);
    inline double calc_midpoint(double, double, double);
    unsigned long zero_search(unsigned long, short range);
    void trigger_playback();
    void pos_wavlen();
    void pos_loopend();
    void pos_wavstart();
    void pos_loopbegin();
    void anti_clip_fwd();
    void anti_clip_rev();
    void ac_copy_fwd_mono(double*);
    void ac_copy_fwd_stereo(st_data*);
    void ac_copy_rev_mono(double*);
    void ac_copy_rev_stereo(st_data*);
    void ac_mix_fwd_mono(double*);
    void ac_mix_fwd_stereo(st_data*);
    void ac_mix_rev_mono(double*);
    void ac_mix_rev_stereo(st_data*);
    // synthmod stuff
    static void create_params();
    static bool done_params;
};

#endif
