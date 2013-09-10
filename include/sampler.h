#ifndef SAMPLER_H
#define SAMPLER_H

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "samplercommon.h"

/* removed all sorts of guff I'd written in 70 lines of comment */

class sampler : public synthmod
{
public:
    sampler(char const*);
    ~sampler();
    // inputs
    void set_input_play_trig(const STATUS* pt ){ in_play_trig = pt;}
    void set_input_stop_trig(const STATUS* st) { in_stop_trig = st;}
    void set_input_deg_size(const double* ds){ in_deg_size = ds;}
    void set_input_start_pos_mod(const double* sp) {
        in_start_pos_mod = sp;
    }
    // outputs
    const short* get_output_left(){ return &out_left;}
    const short* get_output_right(){ return &out_right;}
    const double* get_output_l(){ return &out_l;}
    const double* get_output_r(){ return &out_r;}
    const STATUS* get_output_loop_trig(){ return &out_loop_trig;}
    const STATUS* get_play_state(){ return &play_state;}
    // params
    void set_wavfilein(wavfilein* wi);
    void set_play_dir(PLAY_DIR pd)   { play_dir = pd;}
    void set_play_mode(PLAY_MODE pm) { play_mode = pm;}
    void set_jump_mode(JUMP_DIR jm) { jump_mode = jm;}
    void set_start_pos_min(unsigned long samps){ min_start_pos = samps;}
    void set_start_pos_max(unsigned long samps){ max_start_pos = samps;}
    void set_loop_begin(unsigned long samps){ loop_begin = samps;}
    void set_loop_end(unsigned long samps){ loop_end = samps;}
    void set_loop_mode(LOOP_MODE lm){ loop_mode = lm;}
    void set_loop_is_offset(STATUS o){ loop_is_offset = o;}
    void set_loop_bi_offset(short lbo){ loop_bi_offset = lbo;}
    void set_anti_clip_samples(short acs);
    void set_anti_clip_each_end(STATUS acee){ ac_each_end = acee;}
    void set_zero_search_range(short zsr) {
        search_range = (zsr != 1) ? zsr : 2;
    }
    void set_degsize_amount(double css){ deg_size_amount = css;} 
    // 0 ignore 1 western  > 0 && < 1 macrotonal
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
    const STATUS* in_play_trig;
    const STATUS* in_stop_trig;
    const double* in_start_pos_mod;
    const double* in_deg_size;
    // outputs
    short out_left;
    short out_right;
    double out_l;
    double out_r;
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
    double deg_size_amount;
    // working
    double root_deg_size;
    PLAY_DIR playdir;
    PLAY_DIR acplaydir;
    bool loop_yet;
    short* mono_buffer;
    short* ac_m_buf;
    stereodata* st_buffer;
    stereodata* ac_st_buf;
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
    WAV_CHANNELS ch;
    unsigned long sampletot;
    // private helper functions:
    inline void fill_buffer(unsigned long);
    inline short calc_midpoint(short, short, double);
    unsigned long zero_search(unsigned long, short range);
    void trigger_playback();
    void pos_wavlen();
    void pos_loopend();
    void pos_wavstart();
    void pos_loopbegin();
    void anti_clip_fwd();
    void anti_clip_rev();
    void ac_copy_fwd_mono(short*);
    void ac_copy_fwd_stereo(stereodata*);
    void ac_copy_rev_mono(short*);
    void ac_copy_rev_stereo(stereodata*);
    void ac_mix_fwd_mono(short*);
    void ac_mix_fwd_stereo(stereodata*);
    void ac_mix_rev_mono(short*);
    void ac_mix_rev_stereo(stereodata*);
    // synthmod stuff
    static int sampler_count;
    static void create_params();
    static bool done_params;
};

#endif
