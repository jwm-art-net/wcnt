#ifndef FADER_H
#define FADER_H

#include "synthmod.h"

class fader: public synthmod::base
{
 public:
    fader(const char*);
    ~fader();

    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool        set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

 private:
    const wcint_t*  in_bar;

    double  out_output;
    STATUS  out_bar_trig;
    wcint_t out_index;
    wcint_t out_count;
    STATUS  out_play_state;

    wcint_t start_bar;
    wcint_t end_bar;
    double  fade_in_time;
    double  fade_out_time;

    samp_t  fade_in_smps;
    samp_t  fismp;
    samp_t  fade_out_smps;
    samp_t  fosmp;

    double  fisz;
    double  fosz;

    int     state;
    void register_ui();
    ui::moditem_list* get_ui_items();

};

#endif
