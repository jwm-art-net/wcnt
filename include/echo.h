#ifndef ECHO_H
#define ECHO_H

#include <math.h>
#include "dtr.h"
#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class echo : public synthmod
{
public:
    echo(char const*);
    ~echo();
    void set_input_signal(const double* in){ in_signal = in;}
    void set_input_gain_mod(const double* gm){ in_gainmod = gm;}
    void set_input_feedback(const double* fb){ in_feedback = fb;}
    void set_input_feedback_mod(const double* fbm){ in_feed_mod = fbm;}
    void set_delay_time(double dt){ delay_time = dt;}
    void set_gain_level(double gl){ gain = gl;}
    void set_gain_modsize(double rms){ gain_modsize = rms;}
    void set_feedback_level(double fbl){ feed_level = fbl;}
    void set_feedback_modsize(double fbms){ feed_modsize = fbms;}
    void set_wetdry(double wd){ wetdry = wd;}
    const double* get_input_signal(){ return in_signal;}
    const double* get_input_gain_mod(){ return in_gainmod;}
    const double* get_input_feedback(){ return in_feedback;}
    const double* get_input_feedback_mod(){ return in_feed_mod;}
    const double* get_output(){ return &output;}
    const double* get_output_wet(){ return &wet_output;}
    double get_delay_time(){ return delay_time;}
    double get_gain_level(){ return gain; }
    double get_gain_modsize(){ return gain_modsize; }
    double get_feedback_level(){ return feed_level;}
    double get_feedback_modsize(){ return feed_modsize;}
    double get_wetdry(){ return wetdry;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    // inputs
    const double* in_signal;
    const double* in_gainmod;
    const double* in_feedback;
    const double* in_feed_mod;
    // output
    double output;
    double wet_output;
    // params
    double delay_time;
    double gain;
    double gain_modsize;
    double feed_level;
    double feed_modsize;
    double wetdry;
    // working
    double* filter;//array
    long filterarraymax;
    long fpos;
    double filtertotal;
    double gainamount;
    double feedamount;
    // synthmod kind of stuff
    static int echo_count;
    void create_params();
    static bool done_params;
};

#endif
