#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <math.h>
#include "dtr.h"
#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

#define FILTERARRAYSIZE 8192

class lpfilter : public synthmod
{
public:
    lpfilter(char const*);
    ~lpfilter();
    void set_input_signal(const double* in){ in_signal = in;}
    void set_input_cutoff_deg_size(const double* ds){ in_deg_size = ds;}
    void set_input_feedback(const double* fb){ in_feedback = fb;}
    void set_input_feedback_mod(const double* fbm){ in_feed_mod = fbm;}
    void set_feedback_level(double fbl){ feed_level = fbl;}
    void set_feedback_modsize(double fbms){ feed_modsize = fbms;}
    void set_wetdry(double wd){ wetdry = wd; }
#ifdef GET_METHODS
    const double* get_input_signal(){ return in_signal;}
    const double* get_input_feedback(){ return in_feedback;}
    const double* get_input_deg_size(){ return in_deg_size;}
    const double* get_input_feedback_mod(){ return in_feed_mod;}
    const double* get_output(){ return &output;}
    const double* get_output_wet(){ return &wet_output;}
    double get_feedback_level(){ return feed_level; }
    double get_feedback_modsize(){ return feed_modsize; }
    double get_wetdry(){ return wetdry;}
#endif
    // virtual funcs
    void run();
    void init(){};
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
#endif	
private:
    // inputs
    const double* in_signal;
    const double* in_deg_size; //cutoff freq deg step size
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
    static int lpfilter_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
