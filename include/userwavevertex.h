#ifndef USERWAVEVERTEX_H
#define USERWAVEVERTEX_H

#include "dobjparamlist.h"

class wv_vtx
{
 public:
	wv_vtx(): deg_pos(0), level(0) {};
	wv_vtx(double dp, double l): deg_pos(dp), level(l) {};
	~wv_vtx(){};
	void set_position(double dp){ deg_pos = dp; }
	void set_level(double lvl){ level = lvl; }
	double get_position(){ return deg_pos; }
	double get_level(){ return level; }
 private:
	double	deg_pos;
	double	level;
};

class wave_vertex : public dobj
{
 public:
	wave_vertex();
	wave_vertex(double udp, double ul, double ldp, double ll);
	~wave_vertex(){};
	wv_vtx* get_lower(){ return &lower;}
	wv_vtx* get_upper(){ return &upper;}
	void set_upper_level(double ul){ upper.set_level(ul);}
	void set_upper_position(double up){ upper.set_position(up);}
	void set_lower_level(double ll){ lower.set_level(ll);}
	void set_lower_position(double lp){ lower.set_position(lp);}
	double get_upper_level(){ return upper.get_level();}
	double get_upper_position(){ return upper.get_position();}
	double get_lower_level(){ return lower.get_level();}
	double get_lower_position(){ return lower.get_position();}
	void modulate(double v_mod, double h_mod);
	#ifndef BARE_MODULES
	bool validate();
	bool set_dparam(dparnames::DPAR_TYPE, void*);	
	void* get_dparam(dparnames::DPAR_TYPE pt);
	#endif
// oh so naughty:	
	wv_vtx output;
 private:
	wv_vtx lower;
	wv_vtx upper;
	#ifndef BARE_MODULES
	void create_dparams();
	static bool done_dparams;
	#endif
};

#endif
