#ifndef STEPPER_H
#define STEPPER_H

#include "stepdata.h"
#include "synthmod.h"
#include "linkedlist.h"

/*   _______
    /===#===\
    |stepper|
    \===#===/
     -------

    inside the stepper is a list of step_data defining the step shape.
    there can be any number of these and they are independant of the
    steppers step_count.

    upon calling init, two arrays are created of size step_count, to these
    arrays is transfered the step shape.

    inside run(), the stepper outputs the first value in the array (after
    modulating between up and lo levels) upon input_trig triggering, the
    next value is output and so on. response time optionally adds a
    little ramp between values if so required. set to 0 to disable the
    rampage.

    in_restart_trig when triggered, tells the stepper that the next time
    it is triggered it should start over from the beginning. if in_trig
    and in_restart trig trigger together, then it immediately begins
    again.

    I forgot to mention the step_data has one position, but two levels
    for modulation purposes, which is what in_modulation is for.
*/

class stepper: public synthmod::base, public linked_list<step_data>
{
public:
    stepper(const char*);
    ~stepper();
    step_data* insert_step(step_data*);
    step_data* insert_step(double pos, double uplvl, double lolvl);

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod::base* duplicate_module(const char* uname, DUP_IO);
    dobj::base* add_dobj(dobj::base*);
private:
    // inputs
    STATUS const* in_trig;
    STATUS const* in_restart_trig;
    double const* in_modulation;
    // params
    wcint_t step_count;
    double up_thresh;
    double lo_thresh;
    double rtime;
    STATUS recycle;
    // outputs
    double out_output;
    // working:
    double output;
    double last_output;
    wcint_t step_no;
    wcint_t next_step_no;
    double* up_levels; // two arrays of size step_count
    double* lo_levels; // hold the step shape split even.
    samp_t rtime_samp;
    samp_t rtime_max_samps;
    double rtime_stpsz;
    double rtime_size;
    void register_ui();
};

#endif
