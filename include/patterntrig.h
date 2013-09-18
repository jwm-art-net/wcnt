#ifndef PATTERNTRIG_H
#define PATTERNTRIG_H

#include "synthmod.h"

/*  from a text pattern string, consisting of an arbitrary length
    of 1's and 0's, outputs a trigger or not, when triggered.
    ie "1010"
    the first trigger would trigger, the second would not, the third
    would, and the fourth wouldn't, the fifth would - would be the first
    again.                                                          */

class patterntrig: public synthmod
{
public:
    patterntrig(char const*);
    ~patterntrig();
    void set_pattern_string(char*);
    // virtual funcs
    void run();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

    stockerrs::ERR_TYPE validate();
private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS out_start_trig;
    STATUS out_end_trig;
    char* pattern;
    char* ptr;
    void init_first();
    
};

#endif
