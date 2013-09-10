#ifndef PATTERNTRIG_H
#define PATTERNTRIG_H

#include "synthmodule.h"
#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

/*
    from a text pattern string, consisting of an arbitrary length
    of 1's and 0's, outputs a trigger or not, when triggered.
    ie "1010"
    the first trigger would trigger, the second would not, the third
    would, and the fourth wouldn't, the fifth would - would be the first
    again.
*/

class patterntrig: public synthmod
{
public:
    patterntrig(char const*);
    ~patterntrig();
    void set_input_trig(STATUS const* it){ in_trig = it; }
    STATUS const* get_input_trig(){ return in_trig;}
    STATUS const* get_output_trig(){ return &out_trig;}
    void set_pattern_string(char*);
    // virtual funcs
    void run();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
#endif
    stockerrs::ERR_TYPE validate();
private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS out_start_trig;
    STATUS out_end_trig;
    char* pattern;
    char* ptr;
    static int patterntrig_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
