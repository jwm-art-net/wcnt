#ifndef METERCHANGE_H
#define METERCHANGE_H

#include "dobjparamlist.h"

// struct timesig used by synthfilereader to read time signature in one go,
// so user can represent time signature as 4/4 or 3/4 etc rather than as
// two parts such as beatsperbar 3 beatvalue 4

struct timesig
{
	short beatsperbar;
	short beatvalue;
};

class meterchange : public dobj
{
 public:
	meterchange();
	meterchange(short br, char btpb, char btval);
 	void set_bar(short br){bar = bar;}
	void set_beatsperbar(char bpb){beatsperbar = bpb;}
	void set_beatvalue(char bv){beatvalue = bv;}
	short get_bar(){ return bar;}
	char get_beatsperbar(){return beatsperbar;}
	char get_beatvalue(){return beatvalue;}
	// virtuals from dobj
	#ifndef BARE_MODULES
	bool validate();
	bool set_dparam(dparnames::DPAR_TYPE, void*);	
	void* get_dparam(dparnames::DPAR_TYPE pt);
	#endif
 private:
	short bar;
 	char beatsperbar;
 	char beatvalue;
	#ifndef BARE_MODULES
	void create_dparams();
	static bool done_dparams;
	#endif
};

#endif
