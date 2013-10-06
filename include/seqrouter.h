#ifndef SEQROUTER_H
#define SEQROUTER_H


#include "synthmod.h"
/*
// seqrouter:
//      a glorified trigrouter module, but serving the purpose of
//      sequencing more readily.
*/

class group;

class seq_router: public synthmod
{
 public:
    seq_router(const char*);
    ~seq_router();

    // virtual funcs
    void run();
    errors::TYPE validate();
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    /* inputs */
    const wcint_t*  in_index;
    const STATUS*   in_note_on_trig;
    const STATUS*   in_note_slide_trig;
    const STATUS*   in_note_off_trig;
    const double*   in_freq;
    const double*   in_velocity;

    /* params */
    wcint_t count;
    STATUS wrap; // ??
    wcint_t index;
    wcint_t last_on_ix;
    wcint_t last_slide_ix;
    wcint_t last_off_ix;

    /* no outputs */

    group* grp;

    STATUS* on_trigs;
    STATUS* slide_trigs;
    STATUS* off_trigs;
    double* vels;
    double* freqs;

    void init_first();
    void create_wcnt_notes();
};

#endif
