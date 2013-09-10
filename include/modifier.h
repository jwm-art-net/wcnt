#ifndef MODIFIER_H
#define MODIFIER_H

#include "dtr.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class modifier : public synthmod 
{
 public:
	modifier(string uname);
	~modifier();
	enum MOD_FUNC // modifier types
	{
		ADD,
		SUB,
		MUL, // multiply
		DIV,
		MOD, //modulus
		SIN,
		COS,
		TAN,
	};
	void set_input_signal(const double* s){ in_signal = s;}
	void set_input_modifier(const double* md){ in_mod = md;}
	const double* get_signal_in(){ return in_signal;}
	const double* get_modifier(){ return in_mod;}
	void set_modifier_func(MOD_FUNC mt){ modifier_func = mt; init(); }
	void set_modifier_size(double ms){ mod_size = ms;}
	void set_min_output_value(double mv){ min_out_val = mv;}
	void set_max_output_value(double mv){ max_out_val = mv;}
	void set_posneg_mirror(STATUS pm){ posnegmirror = pm;}
	MOD_FUNC get_modifier_type(){ return modifier_func;}
	double get_modifier_size(){ return mod_size;}
	double get_min_output_value(){ return min_out_val;}
	double get_max_output_value(){ return max_out_val;}
	STATUS get_posneg_mirror(){ return posnegmirror;}
	// virtual funcs
	void run();
	void init();
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	
 private:
	const double* in_signal;
	const double* in_mod;
	double out_output;
	MOD_FUNC modifier_func;
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
	double func_add(){ return *in_signal + *in_mod * mod_size;}
	double func_sub(){ return *in_signal - *in_mod * mod_size;}
	double func_mul(){ return *in_signal * *in_mod * mod_size;}
	double func_div()
	{
		if (*in_mod * mod_size == 0) 
			return (*in_signal < 0) ? -max_out_val : max_out_val; 
		return *in_signal / (*in_mod * mod_size);
	}
	double func_mod()
	{ 
		if (*in_mod * mod_size == 0) 
			return (*in_signal < 0) ? -max_out_val : max_out_val; 
		return fmod(*in_signal, *in_mod * mod_size);
	}
	double func_sin(){ return *in_signal * sin(*in_mod * mod_size * DTR);}
	double func_cos(){ return *in_signal * cos(*in_mod * mod_size * DTR);}
	double func_tan(){ return *in_signal * tan(*in_mod * mod_size * DTR);}
	static int modifier_count;
	static void create_params();
	static bool done_params;
};

#endif
