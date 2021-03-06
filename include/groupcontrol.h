#ifndef GROUPCONTROL_H
#define GROUPCONTROL_H

#include "wcnttrigger.h"

/*
// group_control
//
//  transfers all the modules of specified group out of the
//  main synthmodlist into it's own private synthmodlist.
//
//  by using in_play_trig and in_stop_trig it determines when to
//  run the modules and when not to run the modules.
//
//  it is intended for (eventual) polyphonic capabilities within
//  wcnt, and only processing modules within a group only when
//  they need to be - ie if the voice.
//
//  in_play_trig will (most probably in most cases) connect to
//  the sequencer's out_note_on_trig. however, in_stop_trig should
//  not connect to the sequencer's out_note_off_trig as that
//  output is usually used to trigger an adsr's release section -
//  it would end processing prematurely...
//
//  so, in_stop_trig should connect to the adsr's out_off_trig instead.
*/

class group;
class synthmodlist;

class group_control: public synthmod::base
{
 public:
    group_control(const char*);
    ~group_control();
    // virtual funcs
    void init();
    void run();
    errors::TYPE validate();
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    /* inputs */
    STATUS const* in_play_trig;
    STATUS const* in_stop_trig;
    /* outputs */
    STATUS out_play_state;
    /* params */
    char* group_name;
    /* working */
    wcint_t stop_pending;
    group* grp;
    synthmod::base** runlist;
    synthmod::base** empty_run_list;
    bool set_group_name(const char*);
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
