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
    enum MOD_FUNC { // modifier types
        ADD,    SUB,    MUL,    DIV,
        MOD,    SIN,    COS,    TAN,
        AND,    OR,     XOR  };
    void set_input_signal(const double* s){ in_signal = s;}
    void set_input_modifier(const double* md){ in_mod = md;}
    const double* get_signal_in(){ return in_signal;}
    const double* get_modifier(){ return in_mod;}
    void set_modifier_func(MOD_FUNC mt){ modifier_func = mt; init(); }
    void set_func_value(double fv){ func_val = fv; }
    void set_modifier_size(double ms){ mod_size = ms;}
    void set_min_output_value(double mv){ min_out_val = mv;}
    void set_max_output_value(double mv){ max_out_val = mv;}
    void set_posneg_mirror(STATUS pm){ posnegmirror = pm;}
    MOD_FUNC get_modifier_type(){ return modifier_func;}
    double get_modifier_size(){ return mod_size;}
    double get_min_output_value(){ return min_out_val;}
    double get_max_output_value(){ return max_out_val;}
    STATUS get_posneg_mirror(){ return posnegmirror;}
    const double * get_output(){return &out_output;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    const double* in_signal;
    const double* in_mod;
    double out_output;
    MOD_FUNC modifier_func;
    double func_val;
    double mod_size;
    double min_out_val;
    double max_out_val;
    STATUS posnegmirror;
    double tmp_out;
    double negmin;
    double posmin;
    double negmax;
    double posmax;
    // modifier type function pointer
    double (modifier::*type_func)();
    // modifier type helper functions
    double func_add() {
        return *in_signal + *in_mod * mod_size;
    }
    double func_sub() {
        return *in_signal - *in_mod * mod_size;
    }
    double func_mul() {
        return *in_signal * (1 - mod_size) + *in_signal 
         * *in_mod * mod_size;
    }
    double func_div() {
        if (mod_size == 0) return *in_signal;
        if (*in_mod == 0) {
            return *in_signal * (1 - mod_size) +
             ((*in_signal < 0) ? -max_out_val : max_out_val) * mod_size;
        }
        return *in_signal * (1 - mod_size) +
         ((*in_signal) / (func_val + *in_mod)) * mod_size;
    }
    double func_mod() {
        if (*in_mod  == 0) {
            return *in_signal * (1 - mod_size)
             + ((*in_signal < 0) ? -max_out_val : max_out_val) * mod_size;
        }
        return *in_signal * (1 - mod_size) +
         fmod(*in_signal * func_val, *in_mod)  * mod_size;
    }
    double func_sin() {
        return *in_signal * (1 - mod_size)
         + *in_signal * sin(*in_mod * func_val * DTR) * mod_size;
    }
    double func_cos() {
        return *in_signal * (1 - mod_size)
         + *in_signal * cos(*in_mod * func_val * DTR) * mod_size;
    }
    double func_tan() {
        return *in_signal * (1 - mod_size)
         + *in_signal * tan(*in_mod * func_val * DTR) * mod_size ;
    }
    double func_and() {
        int insig = (int)(*in_signal * func_val);
        int msig = (int)(*in_mod * func_val);
        return *in_signal * (1 - mod_size) +
         ((insig & msig) / func_val) * mod_size;
    }
    double func_or() {
        int insig = (int)(*in_signal * func_val);
        int msig = (int)(*in_mod * func_val);
        return *in_signal * (1 - mod_size) +
         ((insig | msig) / func_val) * mod_size;
    }
    double func_xor() {
        int insig = (int)(*in_signal * func_val);
        int msig = (int)(*in_mod * func_val);
        return *in_signal * (1 - mod_size) +
         ((insig ^ msig) / func_val) * mod_size;
    }
    static int modifier_count;
    void create_params();
    static bool done_params;
};

#endif
