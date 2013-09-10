#ifndef WCNTEXIT_H
#define WCNTEXIT_H

#include "synthmod.h"

/*
wcnt_exit

this module is purely in existance for jwmsynth::execute() to
grab a out_bar from a single instance of the time_map synthmod 
class, and to know which bar to stop at.

it does not serve any other purpose, which is why it does 
absolutely nothing.

because: some wierdo might create more than one time_map module
in their wcnt file, that same wierdo must also specify which one
to use.  

Discounted option: unless of course, jwmsynth searched through the 
module list for time_map modules, and somehow worked out which one 
would cause the longest execution. then of course it may be running
longer than necessary.  I'd rather leave the responsibility on the 
user to decide.

Talking shite as usual are n't you james, eh?  You did not do either
of those choices, in the end.  The user is not allowed to create more
than one of these with wcnt complaining like the old wind/winge bag
it is. (hmmm, talking to yourself again - yes i am).

*/


class wcnt_exit: public synthmod
{
public:
    wcnt_exit(char const*);
    ~wcnt_exit();
    // virtual funcs
    void run(){};
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const { return 0; }
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
private:
    // inputs
    const short* in_bar;
    // no outputs
    // params
    short exit_bar;
    // synthmod stuff
    void create_params();
    static bool done_params;
};

#endif
