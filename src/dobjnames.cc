#ifndef DOBJNAMES_H
#include "../include/dobjnames.h"

#ifndef BARE_MODULES

dobjnames::dobjnames()
{
	dobj_name = new string[DOBJ_LAST];
	dobj_name[DOBJ_FIRST] = "**BAD OBJECT**";
	dobj_name[DOBJ_NONE] = "none";
	dobj_name[DOBJ_INLISTS] = "lists_defined_in_modules";
	dobj_name[LIN_ENVELOPE] = "envelope";
	dobj_name[LIN_WAVEFORM] = "waveform"; 
	dobj_name[LIN_METER] = "meter_map";	
	dobj_name[LIN_BPM] = "bpm_map";	
	dobj_name[LIN_TRACK] = "track";
	dobj_name[LIN_MIX] = "mix";
	dobj_name[LIN_SIGNALS] = "signals";
	dobj_name[LIN_DYNAMICS] = "amp_map";
	dobj_name[DOBJ_INSINGLES] = "objects_defined_in_lists";
	dobj_name[SIN_NOTE] = "note";
	dobj_name[SIN_COORD] = "section";
	dobj_name[SIN_VERTEX] = "vertex";
	dobj_name[SIN_METER] = "meter";
	dobj_name[SIN_BPM] = "tempo";
	dobj_name[SIN_RIFFNODE] = "riff_node";
	dobj_name[SIN_DVERTEX] = "dvertex";
	dobj_name[DOBJ_DEFLISTS] = "lists_defined_outside_modules";
	dobj_name[LDEF_RIFF] = "riff";		
	dobj_name[LDEF_SAMPLESET] = "sample_set";	
	dobj_name[LDEF_RULESET] = "rule_set";	
	dobj_name[DOBJ_INSINGLES] = "objects_defined_outside_lists";
	dobj_name[SDEF_WAVFILEIN] = "wavfilein";
	dobj_name[SDEF_SAMPLEDATA] = "sampledata";
	dobj_name[SDEF_MAPRULE] = "rule";
	dobj_name[DOBJ_SYNTHMOD] = "synthmod";
}


dobjnames::~dobjnames()
{
	delete [] dobj_name;
}

string & dobjnames::get_name(dobjnames::DOBJ_TYPE dt)
{
	return dobj_name[check_type(dt)];
}

dobjnames::DOBJ_TYPE dobjnames::get_type(string const* dname)
{
	if (dname == 0) return DOBJ_FIRST;
	for (int dt = DOBJ_FIRST + 1; dt < DOBJ_LAST; dt++) {
		if (dt == DOBJ_INLISTS || dt == DOBJ_INSINGLES || 
			dt == DOBJ_DEFLISTS || dt == DOBJ_DEFSINGLES) dt++;
		if (*dname == dobj_name[dt])
			return (DOBJ_TYPE)dt;
	}
	return DOBJ_FIRST;
}

dobjnames::DOBJ_TYPE dobjnames::check_type(dobjnames::DOBJ_TYPE dt)
{
	if (dt <= DOBJ_FIRST ||
		dt == DOBJ_NONE ||
		dt == DOBJ_INLISTS ||
		dt == DOBJ_INSINGLES ||
		dt == DOBJ_DEFLISTS ||
		dt == DOBJ_DEFSINGLES ||
		dt >= DOBJ_LAST) 
		return DOBJ_FIRST;
	return dt;
}

dobjnames::DOBJ_TYPE dobjnames::get_sub_type(dobjnames::DOBJ_TYPE dt)
{
	if (dt >= DOBJ_LAST)		return DOBJ_FIRST;
	if (dt == DOBJ_SYNTHMOD)	return DOBJ_SYNTHMOD;
	if (dt >= DOBJ_DEFSINGLES)	return DOBJ_DEFSINGLES;
	if (dt >= DOBJ_DEFLISTS)	return DOBJ_DEFLISTS;
	if (dt >= DOBJ_INSINGLES)	return DOBJ_INSINGLES;
	if (dt >= DOBJ_INLISTS)		return DOBJ_INLISTS;
	return DOBJ_FIRST;
}

string & dobjnames::get_sub_name(dobjnames::DOBJ_TYPE dt)
{
	return dobj_name[get_sub_type(dt)];
}

#endif
#endif
