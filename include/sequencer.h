#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "riffnode.h"
#include "timemap.h" 

#ifndef BARE_MODULES
#include "dobjlist.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class sequencer : public synthmod
{
public:
    sequencer(char const*);
    ~sequencer();
    riff_node* add_riff(string const* riffname, short barpos);
    riff_node* add_riff(riffdata*, short barpos);
    riff_node* add_riff_node(riff_node*);
    bool delete_riff_node(riff_node* rn);
    riff_node* goto_first() {
        return (riff_node*)riffnodelist->goto_first()->get_data(); 
    }
    riff_node* goto_last() {
        return (riff_node*)riffnodelist->goto_last()->get_data();
    }
    riff_node* goto_prev() {
        return (riff_node*)riffnodelist->goto_prev()->get_data();
    }
    riff_node* goto_next() {
        return (riff_node*)riffnodelist->goto_next()->get_data();
    }
    // set inputs
    void set_input_bar_trig(const STATUS* bt){ in_bar_trig = bt;}
    void set_input_bar(const short* b){ in_bar = b;}
    void set_input_pos_step_size(const double* bs) {
        in_pos_step_size = bs;
    }
    void set_input_beats_per_bar(const short* bpb) {
        in_beats_per_bar = bpb;
    }
    void set_input_beat_value(const short* bv){ in_beat_value = bv;}
    // get outputs
    const STATUS* get_output_note_on_trig() { return &out_note_on_trig; }
    const STATUS* get_output_note_slide_trig() {
        return &out_note_slide_trig;
    }
    const STATUS* get_output_note_off_trig() {
        return &out_note_off_trig; 
    }
    const STATUS* get_output_riff_start_trig() {
        return &out_riff_start_trig;
    }
    const STATUS* get_output_riff_end_trig() {
        return &out_riff_end_trig; 
    }
    const STATUS* get_output_end_trig() { return &out_end_trig; }
    const double* get_output_freq() { return &out_freq; }
    const double* get_output_velocity() { return &out_velocity; }
    const double* get_output_velocity_ramp() {return &out_velocity_ramp;}
    const STATUS* get_riff_play_state() { return &riff_play_state; }
    const STATUS* get_note_play_state() { return &note_play_state; }
    const char ** get_output_notename(){ return &out_notename;}
    // params
    void set_velocity_response_time(double ms){vel_response = ms;}
    void set_hold_notename(STATUS hn){hold_notename = hn;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
    dobj* add_dobj(dobj*);
#endif
private:
    //inputs
    const STATUS* in_bar_trig;
    const short* in_bar;
    const double* in_pos_step_size;
    const short* in_beats_per_bar;
    const short* in_beat_value;
    /*outpduts*/
    STATUS out_note_on_trig;
    STATUS out_note_slide_trig;
    STATUS out_note_off_trig;
    STATUS out_riff_start_trig;
    STATUS out_riff_end_trig;
    STATUS out_start_trig;
    STATUS out_end_trig;
    const char* out_notename;
    double out_freq;
    double out_velocity;
    double out_velocity_ramp;
    short out_transpose;
    STATUS riff_play_state;
    STATUS note_play_state;
    // params
    double vel_response;
    STATUS hold_notename;
    /* the riff node list */
    linkedlist* riffnodelist;
    /* working data */
    riff_node* cur_node;
    riff_node* riff_node_ptr;
    riffdata* riff_ptr;
    ll_item* riffnodeitem;
    note_data* note_ptr;
    ll_item* noteitem;
    char zero; // out_note points to this when no note playing.
    short riff_start_bar;
    double barpos;
    long note_on_pos; 	// i did use shorts, but with each riff specifying
    double note_on_ratio; // note_on_pos / riff_length
    double len_offset;	// adds to note length
    long note_off_pos;	// 1/4 value, if it's large like 768 or something
    double posconv;    	// notes in riffs > 16 bars long may get lost...
    unsigned long velrsp_max_samps;
    unsigned long velrsp_samp;
    double vel_stpsize;
    STATUS start_pending;
    STATUS end_pending;
    /* helper functions */
    riff_node* remove(riff_node * rn);
    void calc_riff_sample_positions();
    void calc_note_sample_positions();
    // synthmod stuff
    static int sequencer_count;
#ifndef BARE_MODULES
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
#endif
};

#endif
