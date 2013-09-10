#ifndef STEPPER_H
#define STEPPER_H

#include "stepdata.h"

#include "conversions.h"
#include "linkedlist.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "moddobjlist.h"
#include "dobjlist.h"

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

class stepper: public synthmod
{
public:
    stepper(char const*);
    ~stepper();
    step_data* insert_step(step_data*);
    step_data* insert_step(double pos, double uplvl, double lolvl);
    bool delete_step(step_data*);
    step_data* goto_first() {
        return step = (step_data*)
         (step_item = steplist->goto_first())->get_data();
    }
    step_data* goto_last() {
        return step = (step_data*)
         (step_item = steplist->goto_last())->get_data();
    }
    step_data* goto_prev() {
        return step = (step_data*)
         (step_item = steplist->goto_prev())->get_data();
    }
    step_data* goto_next() {
        return step = (step_data*)
         (step_item = steplist->goto_next())->get_data();
    }
    // inputs
    void set_input_trig(STATUS const* it){ in_trig = it;}
    void set_input_restart_trig(STATUS const* rt){ in_restart_trig = rt;}
    void set_input_modulation(double const* im){ in_modulation = im;}
    STATUS const* get_input_trig(){ return in_trig;}
    STATUS const* get_input_restart_trig(){ return in_restart_trig;}
    double const* get_input_modulation(){ return in_modulation;}
    // params
    void set_step_count(short sc){ step_count = sc; }
    void set_upper_thresh(double ut){ up_thresh = ut;}
    void set_lower_thresh(double lt){ lo_thresh = lt;}
    void set_response_time(double rt){ rtime = rt;}
    void set_recycle_mode(STATUS rm){ recycle = rm; }
    short get_step_count(){ return step_count;}
    double get_upper_thresh(){ return up_thresh;}
    double get_lower_thresh(){ return lo_thresh;}
    double get_response_time(){ return rtime;}
    STATUS get_recycle_mode(){ return recycle; }
    // outputs
    double const* get_output(){ return &out_output;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);
private:
    // inputs
    STATUS const* in_trig;
    STATUS const* in_restart_trig;
    double const* in_modulation;
    // params
    short step_count;
    double up_thresh;
    double lo_thresh;
    double rtime;
    STATUS recycle;
    // outputs
    double out_output;
    // working:
    double output;
    double last_output;
    short step_no;
    short next_step_no;
    double* up_levels; // two arrays of size step_count
    double* lo_levels; // hold the step shape split even.
    unsigned long rtime_samp;
    unsigned long rtime_max_samps;
    double rtime_stpsz;
    double rtime_size;
    // the step list and associates:
    linkedlist* steplist;
    ll_item*    step_item;
    step_data*  step;
    // interface stuff
    static int stepper_count;
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
