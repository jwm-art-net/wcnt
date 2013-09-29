#ifndef TRIGROUTER_H
#define TRIGROUTER_H

#include "wcnttrigger.h"

/*
// trigrouter:
//  creates an array of wcnt_trigger modules and adds them to a group
//  (group has same name as the trigrouter's username). each wcnt_trigger
//  is given a numerical name.
//  and a list of hidden outputs (not available to user)
//  connects the wcnt_trigger modules using the hidden outputs
//  
*/

class group;

class trigrouter: public synthmod
{
public:
    trigrouter(const char*);
    ~trigrouter();
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    /* inputs */
    STATUS const* in_trig;
    short  const* in_count;
    /* params */
    short count;
    STATUS wrap;
    /* no outputs */
    group* grp;
    STATUS* trigs;
    void init_first();
    void create_wcnt_triggers();
    
};

#endif
