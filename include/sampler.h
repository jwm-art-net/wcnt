#ifndef SAMPLER_H
#define SAMPLER_H

#include "samplercommon.h"
#include "synthmod.h"

/* removed all sorts of guff I'd written in 70 lines of comment */

class wavfilein;
class st_data;

class sampler : public synthmod::base
{
 public:
    sampler(const char*);
    ~sampler();
    // virtual funcs
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
    samp_t min_start_pos;
    samp_t max_start_pos;
    samp_t loop_begin;
    samp_t loop_end;
    STATUS loop_is_offset;
    LOOP_MODE loop_mode;
    wcint_t loop_bi_offset;
    wcint_t xfade_samples;
    STATUS xfade_each_end;
    wcint_t search_range;
    double phase_step_amount;
    // working
    double root_phase_step;
    PLAY_DIR playdir;
    PLAY_DIR acplaydir;
    bool loop_yet;
    double* mono_buffer;
    double* xf_m_buf;
    st_data* st_buffer;
    st_data* xf_st_buf;
    samp_t buffer_start_pos;
    int buff_pos;
    samp_t wavstart;
    samp_t wavlength;
    samp_t wavstbi;
    samp_t wavlenbi;
    double cur_pos;
    double xf_cpstep;
    double oldcpstep;
    double cp_step;
    double cp_ratio;
    double sr_ratio; //ratio between input and output samplerates
    double bp_midpoint;
    samp_t start_pos;
    samp_t startpos_span;
    samp_t loopstart;
    samp_t loopfinish;
    bool loop_fits_in_buffer;
    bool loop_loaded;
    STATUS do_ac;
    double xf_cur_pos;
    double xf_midpoint;
    int xf_buf_pos;
    samp_t xf_buf_start_pos;
    double xf_step;
    double xf_size;
    double xf_out_left;
    double xf_out_right;
    wcint_t ch;
    samp_t sampletot;
    // private helper functions:
    inline void fill_buffer(samp_t);
    inline double calc_midpoint(double, double, double);
    samp_t zero_search(samp_t, wcint_t range);
    void trigger_playback();
    void pos_wavlen();
    void pos_loopend();
    void pos_wavstart();
    void pos_loopbegin();
    void xfade_fwd();
    void xfade_rev();
    void xf_copy_fwd_mono(double*);
    void xf_copy_fwd_stereo(st_data*);
    void xf_copy_rev_mono(double*);
    void xf_copy_rev_stereo(st_data*);
    void xf_mix_fwd_mono(double*);
    void xf_mix_fwd_stereo(st_data*);
    void xf_mix_rev_mono(double*);
    void xf_mix_rev_stereo(st_data*);
    // synthmod stuff
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
