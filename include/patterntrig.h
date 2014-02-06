#ifndef PATTERNTRIG_H
#define PATTERNTRIG_H

#include "synthmod.h"

/*  from a text pattern string, consisting of an arbitrary length
    of 1's and 0's, outputs a trigger or not, when triggered.
    ie "1010"
    the first trigger would trigger, the second would not, the third
    would, and the fourth wouldn't, the fifth would - would be the first
    again.                                                          */

class patterntrig: public synthmod::base
{
public:
    patterntrig(const char*);
    ~patterntrig();
    void set_pattern_string(const char*);
    // virtual funcs
    void run();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

    errors::TYPE validate();
private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS out_start_trig;
    STATUS out_end_trig;
    wcint_t out_index;

    char* pattern;
    char* ptr;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
