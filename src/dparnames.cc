#ifndef DPARNAMES_H
#include "../include/dparnames.h"

dparnames::dparnames() :
	dparname(0), dparcat(0)
{
	dparname = new string[DPAR_LAST];
	dparcat = new IOCAT[DPAR_LAST];
	dparname[DPAR_FIRST] = "Bad Dparam Type";	dparcat[DPAR_FIRST] = CAT_FIRST;
	dparname[DPAR_QUARTERVAL] = "quarter_value";dparcat[DPAR_QUARTERVAL] = CAT_SHORT;
	dparname[DPAR_NOTENAME] = "name";			dparcat[DPAR_NOTENAME] = CAT_STRING;
	dparname[DPAR_NOTEPOS] = "pos";				dparcat[DPAR_NOTEPOS] = CAT_DOUBLE;
	dparname[DPAR_NOTELEN] = "len";				dparcat[DPAR_NOTELEN] = CAT_DOUBLE;
	dparname[DPAR_NOTEVEL] = "vel";				dparcat[DPAR_NOTEVEL] = CAT_DOUBLE;
	dparname[DPAR_ADSRSECT] = "name";			dparcat[DPAR_ADSRSECT] = CAT_ADSRSECT;
	dparname[DPAR_UPTIME] = "ut";				dparcat[DPAR_UPTIME] = CAT_DOUBLE;
	dparname[DPAR_LOTIME] = "lt";				dparcat[DPAR_LOTIME] = CAT_DOUBLE;
	dparname[DPAR_UPLEVEL] = "ul";				dparcat[DPAR_UPLEVEL] = CAT_DOUBLE;
	dparname[DPAR_LOLEVEL] = "ll";				dparcat[DPAR_LOLEVEL] = CAT_DOUBLE;
	dparname[DPAR_UPDEG] = "ud";				dparcat[DPAR_UPDEG] = CAT_DOUBLE;
	dparname[DPAR_LODEG] = "ld";				dparcat[DPAR_LODEG] = CAT_DOUBLE;
	dparname[DPAR_METER] = "tsig";				dparcat[DPAR_METER] = CAT_METER;
	dparname[DPAR_BAR] = "bar";					dparcat[DPAR_BAR] = CAT_SHORT;
	dparname[DPAR_BPM] = "bpm";					dparcat[DPAR_BPM] = CAT_DOUBLE;
	dparname[DPAR_RIFFNAME] = "riff";			dparcat[DPAR_RIFFNAME] = CAT_DOBJ;
	dparname[DPAR_MODNAME] = "name";			dparcat[DPAR_MODNAME] = CAT_SYNTHMOD;
	dparname[DPAR_SAMPLESETNAME] = "sample_set";dparcat[DPAR_SAMPLESETNAME] = CAT_DOBJ;
	dparname[DPAR_SAMPLENO] = "sample_no";		dparcat[DPAR_SAMPLENO] = CAT_SHORT;
	dparname[DPAR_FILENAME] = "filename";		dparcat[DPAR_FILENAME] = CAT_STRING;
	dparname[DPAR_ROOTNOTE] = "root_note";		dparcat[DPAR_ROOTNOTE] = CAT_STRING;
	dparname[DPAR_TRANSPOSE] = "transpose";		dparcat[DPAR_TRANSPOSE] = CAT_SHORT;
	dparname[DPAR_STARTLEN] = "start_len";		dparcat[DPAR_STARTLEN] = CAT_DOUBLE;
	dparname[DPAR_ENDLEN] = "end_len";			dparcat[DPAR_ENDLEN] = CAT_DOUBLE;
	dparname[DPAR_VALUE] = "value";				dparcat[DPAR_VALUE] = CAT_DOUBLE;
	dparname[DPAR_SIGIN_LEVEL] = "sigin_level";	dparcat[DPAR_SIGIN_LEVEL] = CAT_DOUBLE;
	dparname[DPAR_SIGOUT_UPLEVEL] = "so_ul";dparcat[DPAR_SIGOUT_UPLEVEL] = CAT_DOUBLE;
	dparname[DPAR_SIGOUT_LOLEVEL] = "so_ll";dparcat[DPAR_SIGOUT_LOLEVEL] = CAT_DOUBLE;
}

dparnames::~dparnames()
{
	delete [] dparname;
	delete [] dparcat;
}

string & dparnames::get_name(dparnames::DPAR_TYPE id)
{
	if (id > DPAR_FIRST && id < DPAR_LAST)
		return dparname[id];
	else
		return dparname[DPAR_FIRST];
}

dparnames::DPAR_TYPE dparnames::get_type(string const* dpname)
{
	if (dpname == 0) return DPAR_FIRST;
	for (int i = DPAR_FIRST + 1; i < DPAR_LAST; i++) {
		if (*dpname == dparname[i])
			return (DPAR_TYPE)i;
	}
	return DPAR_FIRST;
}

IOCAT dparnames::get_category(dparnames::DPAR_TYPE id)
{
	if (id > DPAR_FIRST && id < DPAR_LAST)
		return dparcat[id];
	else
		return dparcat[DPAR_FIRST];
}

#endif
