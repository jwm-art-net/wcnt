#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "riffnode.h"
#include "timemap.h" 

#include "dobjlist.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "moddobjlist.h"

/*
    wcnt-1.128
    ----------

    enhanced note slide implimentation.

    (a note slide occurrs when two or more notes overlap. instead of
    triggering note on, note slide is triggered for the second note.)

    previously, if the first note ended after the second note,
    the rest of the first note would be lost. now, the rest
    of the note is played.  every note in a riff is placed at the
    end of a list (play_list), once the note has finished it is
    removed from the list. this allows the sequencer to find out
    if, when a note ends, it should switch back to a previous note,
    or to wait until the next note should be played.

    BTW: just because the sequencer now has a play list don't think
         it has become anything more than monophonic.
*/

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
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
    dobj* add_dobj(dobj*);
private:
    // ***** inputs *****
    const STATUS* in_bar_trig;
    const short* in_bar;
    const double* in_pos_step_size;
    const short* in_beats_per_bar;
    const short* in_beat_value;
    // ***** outpduts ******
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
    // ***** params *****
    double vel_response;
    // ***** the riff node list *****
    linkedlist* riffnodelist;
    // ***** working data *****
    riff_node* cur_node;
    riff_node* riff_node_ptr;
    riffdata* riff_ptr;
    ll_item* riffnodeitem;
    short riff_start_bar;
    double riff_pos;
    double riff_len;
    double posconv;
    unsigned long velrsp_max_samps;
    unsigned long velrsp_samp;
    double vel_stpsize;
    STATUS start_pending;
    STATUS end_pending;
    // ***** new play_list and note stuff *****
    linkedlist* play_list;
    ll_item* play_item;
    ll_item* next_in_riff;
    note_data* play_note;
    note_data* next_note;
    note_data* note_ptr;
    double next_note_on_pos;
    double play_note_off_pos;
    // ***** helper methods *****
    // position conversion to that used by time_map, transpose etc.
    note_data* posconv_note(note_data* riff_note);
    void init_next_note(ll_item* riff_note_item);
    void output_note(note_data* note);
    // synthmod stuff
    static int sequencer_count;
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
