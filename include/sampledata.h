#ifndef SAMPLEDATA_H
#define SAMPLEDATA_H

#include "samplercommon.h"
#include "wavfilein.h"
#include "dobjparamlist.h"

// sample_data - these are inserted into sample_set.
// when note_data is extended it will specify which sample set to use.
// the mapper object will map the samples in the set, by sample
// index no, to the notenames output by sequencer

// unfinished as yet ;/ cause it sa wee beace tea

class sample_data : public dobj
{
public:
    sample_data();
    ~sample_data(){};
    void set_sdataname(const char* sdn);
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
    void set_loop_bi_offset(int lbo){ loop_bi_offset = lbo;}
    void set_root_note(char * rn);
    char* get_sdataname(){ return sdataname;}
    wavfilein* get_wavfilein(){ return wavfile;}
    PLAY_DIR get_play_dir(){ return play_dir;}
    PLAY_MODE set_play_mode() { return play_mode;}
    JUMP_DIR set_jump_mode() { return jump_mode;}
    unsigned long set_start_pos_min(){ return min_start_pos;}
    unsigned long set_start_pos_max(){ return max_start_pos;}
    LOOP_MODE set_loop_mode(){ return loop_mode;}
    STATUS set_loop_is_offset(){ return loop_is_offset;}
    unsigned long set_loop_begin(){ return loop_begin;}
    unsigned long set_loop_end(){ return loop_end;}
    int set_loop_bi_offset(){ return loop_bi_offset;}
    void set_root_note(char);
// working: these are called by some other class, probably a module
// in order to get the data. there is only get_start_pos and not
// the min and max versions. get_loop_begin/end when loop_is_offset=ON
// return the position offset from modulated start pos, not value set
// run(ptr_to_signal) sets the value returned by former mentioned methods
    void run(double* start_pos_mod);
    unsigned long get_start_pos() { return start_pos; }
    unsigned long get_loop_start_pos(){ return loop_start_pos;}
    unsigned long get_loop_end_pos(){ return loop_end_pos;}
#ifndef BARE_MODULES
    virtual stockerrs::ERR_TYPE validate();
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE pt);
#endif
private:
    // parameters
    char* sdataname;
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
    int loop_bi_offset;
    double root_deg_size; // used by root note/freq
    // working
    unsigned long start_pos;
    unsigned long loop_start_pos;
    unsigned long loop_end_pos;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
