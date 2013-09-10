#ifndef USERWAVEVERTEX_H
#define USERWAVEVERTEX_H

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

class wave_vertex
{
 public:
	wave_vertex();
	wave_vertex(double udp, double ul, double ldp, double ll);
	~wave_vertex(){};
	wv_vtx* get_lower(){ return &lower;}
	wv_vtx* get_upper(){ return &upper;}
	// cant imagine what this is all about:
	double oi_get_position(){ return lower.get_position();}
	// actually oi is shorthand for ordered insert, ie this is the
	// function used by ordered insert.
	void modulate(double v_mod, double h_mod);
	wv_vtx output;
 private:
	wv_vtx lower;
	wv_vtx upper;
};

#endif
