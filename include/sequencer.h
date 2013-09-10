#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "riffnode.h"
#include "synthmod.h"

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
         it has become anything other than monophonic.

    wcnt-1.25
    ---------
    have removed the length changing stuff from here and riff_node

*/

class sequencer : public synthmod, public linked_list<riff_node>
{
 public:
    sequencer(char const*);
    ~sequencer();
/*
    unused:
    riff_node* add_riff(const char* const riffname, short barpos);
    riff_node* add_riff(riffdata*, short barpos);
*/
    riff_node* add_riff_node(riff_node*);

    bool delete_riff_node(riff_node* rn);

    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO dupio);
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
    short start_bar;
    double vel_response;
    // ***** working data *****
    riff_node* cur_node;
    riff_node* riff_node_ptr;
    riffdata* riff_ptr;
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
    linked_list<note_data>* play_list;
    ll_item<note_data>* play_item;
    ll_item<note_data>* next_in_riff;
    note_data* play_note;
    note_data* next_note;
    note_data* note_ptr;
    double next_note_on_pos;
    double play_note_off_pos;
    // ***** helper methods *****
    // position conversion to that used by time_map, transpose etc.
    note_data* posconv_note(note_data* riff_note);
    void init_next_note(ll_item<note_data>* riff_note_item);
    void output_note(note_data* note);
    // synthmod stuff
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
