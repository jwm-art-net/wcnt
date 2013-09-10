#ifndef LOGICTRIGGER_H
#define LOGICTRIGGER_H

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class logictrigger: public synthmod 
{
 public:
	enum LOGIC_FUNC{ AND = 1, OR, XOR };
	logictrigger(string uname);
	~logictrigger();
	void set_input_trig1(STATUS const* it){ in_trig1 = it; }
	void set_input_trig2(STATUS const* it){ in_trig2 = it; }
	STATUS const* get_input_trig1(){ return in_trig1;}
	STATUS const* get_input_trig2(){ return in_trig2;}
	STATUS const* get_output_trig(){ return &out_trig;}
	void set_logicfunc(LOGIC_FUNC ft) { logicfunc = ft;}
	LOGIC_FUNC get_logicfunc(){ return logicfunc;}
	// virtual funcs
	void run();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
		
 private:
	STATUS const* in_trig1;
	STATUS const* in_trig2;
	STATUS out_trig;
	LOGIC_FUNC logicfunc;
	static int logictrigger_count;
	#ifndef BARE_MODULES
	void create_params();
	static bool done_params;
 	#endif
};

#endif
