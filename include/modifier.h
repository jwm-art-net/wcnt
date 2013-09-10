#ifndef MODIFIER_H
#define MODIFIER_H

#include "dtr.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class modifier : public synthmod
{
public:
    modifier(char const*);
    ~modifier();
    enum MOD_FUNC { ADD, SUB};
    void set_input_signal(const double* s){ in_signal = s;}
    void set_input_modifier(const double* md){ in_mod = md;}
    const double* get_input_signal(){ return in_signal;}
    const double* get_input_modifier(){ return in_mod;}
    void set_func(MOD_FUNC f){ func = f; }
    void set_bias(double b){ bias = b;}
    MOD_FUNC get_func(){ return func;}
    double get_bias(){ return bias;}
    // output
    const double * get_output(){return &out_output;}
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    const double* in_signal;
    const double* in_mod;
    double out_output;
    MOD_FUNC func;
    double bias;
    static int modifier_count;
    void create_params();
    static bool done_params;
};

#endif
