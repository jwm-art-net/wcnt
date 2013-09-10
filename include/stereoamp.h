#ifndef STEREOAMP_H
#define STEREOAMP_H

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class stereo_amp : public synthmod
{
public:
    stereo_amp(char const*);
    ~stereo_amp();
    void set_input_left(const double* inl){ in_left = inl; }
    void set_input_right(const double* inr){ in_right = inr; }
    void set_input_amp_eg(const double* aeg){ in_amp_eg = aeg; }
    void set_input_amp_mod(const double* am){ in_amp_mod = am; }
    const double* get_input_left(){ return in_left; }
    const double* get_input_right(){ return in_right; }
    const double* get_input_amp_eg(){ return in_amp_eg; }
    const double* get_input_amp_mod(){ return in_amp_mod; }
    const short* get_output_left(){ return &out_left; }
    const short* get_output_right(){ return &out_right; }
    void set_left_amplitude(short la){ left_amplitude = la; }
    void set_right_amplitude(short ra){ right_amplitude = ra; }
    void set_amp_modsize(double ams){ amp_modsize = ams; }
    void set_clip_level(short cl){ clip_level = cl; }
    short get_left_amplitude(){ return left_amplitude; }
    short get_right_amplitude(){ return right_amplitude; }
    double get_amp_modsize(){ return amp_modsize; }
    short get_clip_level(){ return clip_level; }
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
    const double* in_left;
    const double* in_right;
    const double* in_amp_eg;
    const double* in_amp_mod;
    // outputs
    short out_left;
    short out_right;
    // params
    short left_amplitude;
    short right_amplitude;
    double amp_modsize;
    short clip_level;
    // working
    double left;
    double right;
    static int stereoamp_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
