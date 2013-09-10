#ifndef DPARAMNAMES_H
#define DPARAMNAMES_H

#include <string.h>

#include "iocat.h"

// these are the param names for accessing the
// methods of dobj inherited classes via
// set_dparam() & get_dparam()

class dparamnames
{
public:
    enum DPAR_TYPE
    {
        DPAR_FIRST = 0,
        DPAR_QUARTERVAL,
        DPAR_NOTENAME,
        DPAR_NOTEPOS,
        DPAR_NOTELEN,
        DPAR_NOTEVEL,
        DPAR_ADSRSECT,
        DPAR_UPTIME,
        DPAR_LOTIME,
        DPAR_UPLEVEL,
        DPAR_LOLEVEL,
        DPAR_UPDEG,
        DPAR_LODEG,
        DPAR_METER,
        DPAR_BAR,
        DPAR_BPM,
        DPAR_RIFFNAME,
        DPAR_MODNAME,
        DPAR_SAMPLESETNAME,
        DPAR_SAMPLENO,
        DPAR_FILENAME,
        DPAR_SAMPLERATE,
        DPAR_ROOTNOTE,
        DPAR_TRANSPOSE,
        DPAR_STARTLEN,
        DPAR_ENDLEN,
        DPAR_VALUE,
        DPAR_SIGIN_LEVEL,
        DPAR_SIGOUT_UPLEVEL,
        DPAR_SIGOUT_LOLEVEL,
        DPAR_SECONDS,
        DPAR_LAST
    };
    dparamnames();
    ~dparamnames();
    char const* get_name(DPAR_TYPE id);
    IOCAT get_category(DPAR_TYPE id);
    DPAR_TYPE get_type(char const*);
private:
    char** dparname;
    IOCAT* dparcat;
};

#endif
