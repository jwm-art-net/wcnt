#ifndef NONEZERO_H
#define NONEZERO_H

#include "synthmod.h"

/* There should be only one..........'cos there's no point 'aving two or
more.  the plan is that it should not be an option for the user to create
them,  infact the user should not be aware that there is such a module.

I've designed it as a module so that when the user selects an output to
plugin, there is the option to have none, or zero, same difference.

    -- actually as far as the user is concerned the input is 'off'

when I programmed in TURBO_C++_v3.00 I found no problem with using NULLL
instead of a pointer to a var whose value was 0 - zero. Using GNU C++
v2.95 using NULL creates seg faults.  But I was vaguely aware that I
needed a better solution so created the nonezero module.......*/

// this class could be very soon becoming deprecated...
// ... favour of shifting the 'outputs' into synthmod itself
// of course, they would also become static... i'm not sure
// yet... other things ta dee first like...
// 7th december 2007

class nonezero: public synthmod
{
public:
    nonezero(const char*);
    ~nonezero();
    const double* get_output_none_double() const {
            return &out_none_double;
        }
    const short* get_output_none_short() const  { return &out_none_short; }
    const unsigned long* get_output_none_ulong() const {
        return &out_none_ulong;
    }
    const STATUS* get_output_none_trig() const  { return &out_none_STATUS; }
    const STATUS* get_output_none_state() const { return &out_none_STATUS; }
    // virtual funcs
    void run(){};
    const void* get_out(outputnames::OUT_TYPE) const;

private:
    const double out_none_double;
    const short out_none_short;
    const unsigned long out_none_ulong;
    const STATUS out_none_STATUS;
    char* out_none_string;
};

#endif
