#ifndef STEREOAMP_H
#define STEREOAMP_H

#include "synthmod.h"

/*
// because we're now using libsndfile to handle file input/output
// we no longer have 16bit integer input/output and amplitude levels
*/

class stereo_amp : public synthmod::base
{
public:
    stereo_amp(const char*);
    ~stereo_amp();
    void run();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    // inputs
    const double* in_left;
    const double* in_right;
    const double* in_amp_eg;
    const double* in_amp_mod;

    // outputs
    double out_left;
    double out_right;

    // params
    double left_level;
    double right_level;
    double amp_modsize;
    double clip_level;

    // working
    double left;
    double right;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
