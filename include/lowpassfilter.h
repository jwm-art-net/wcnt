#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include "synthmodule.h"

class lpfilter : public synthmod
{
 public:
    lpfilter(char const*);
    ~lpfilter();
    void run();
    void init(){};
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
 private:
    // inputs
    const double* in_signal;
    const double* in_phase_step; //cutoff freq deg step size
    const double* in_feedback;
    const double* in_feed_mod;
    // output
    double output;
    double wet_output;
    // params
    double feed_level;
    double feed_modsize;
    double wetdry;
    // working
    double* filter;//array
    short filterarraymax;
    short fpos;
    double filtertotal;
    double feedback;
    void create_params();
    static bool done_params;
};

#endif
