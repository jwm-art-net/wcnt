#ifndef METERCHANGE_H
#define METERCHANGE_H

#include "dobjparamlist.h"

// struct timesig used by synthfilereader to read time signature in one
// go, so user can represent time signature as 4/4 or 3/4 etc rather than
// as two parts such as beatsperbar 3 beatvalue 4

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
    void set_bar(short br){bar = br;}
    void set_beatsperbar(char bpb){time_sig.beatsperbar = bpb;}
    void set_beatvalue(char bv){time_sig.beatvalue = bv;}
    short get_bar(){ return bar;}
    char get_beatsperbar(){return time_sig.beatsperbar;}
    char get_beatvalue(){return time_sig.beatvalue;}
    // virtuals from dobj
#ifndef BARE_MODULES
    stockerrs::ERR_TYPE validate();
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE pt);
#endif
private:
    short bar;
    timesig time_sig;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
