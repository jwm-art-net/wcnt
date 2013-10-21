#ifndef TRIGROUTER_H
#define TRIGROUTER_H

#include "synthmod.h"
/*
// trigrouter:
//  creates an array of wcnt_trigger modules and adds them to a group
//  (group has same name as the trigrouter's username). each wcnt_trigger
//  is given a numerical name.
//  and a list of hidden outputs (not available to user)
//  connects the wcnt_trigger modules using the hidden outputs.
//
//  note:   because wcnt_trigger modules have an empty run method, they
//          are removed from the run list and consequently do not cost
//          anything in terms of performance.
*/

class group;

class trigrouter: public synthmod::base
{
 public:
    trigrouter(const char*);
    ~trigrouter();
    // virtual funcs
    void run();
    errors::TYPE validate();
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    /* inputs */
    const STATUS* in_trig;
    const wcint_t* in_index;
    const STATUS* in_all_off_trig;
    /* params */
    wcint_t count;
    STATUS wrap;
    /* no outputs */
    group* grp;
    STATUS* trigs;
    wcint_t last_ix;
    void create_wcnt_triggers();
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
