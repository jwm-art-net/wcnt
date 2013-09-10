#ifndef USERWAVEVERTEX_H
#include "../include/userwavevertex.h"

wave_vertex::wave_vertex()
: output(0, 0), lower(0, 0), upper(0, 0)
{
}

wave_vertex::wave_vertex(double udp, double ul, double ldp, double ll)
: output(0, 0), lower(ldp, ll), upper(udp, ul)
{
}
	
void wave_vertex::modulate(double v_mod, double h_mod)
{
	double up_pos = upper.get_position();
	double lo_pos = lower.get_position();
	double up_lvl = upper.get_level();
	double lo_lvl = lower.get_level();
	output.set_position(lo_pos + (up_pos - lo_pos) * ((h_mod < 0) ? -h_mod : h_mod));
	output.set_level(lo_lvl + (up_lvl - lo_lvl) * ((v_mod < 0) ? -v_mod : v_mod));
}

#endif
