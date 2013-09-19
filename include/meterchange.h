#ifndef METERCHANGE_H
#define METERCHANGE_H

#include "dobj.h"

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
    // these accessor methods are used by timemap
    void set_bar(short br)          { bar = br; }
    void set_beatsperbar(char bpb)  { time_sig.beatsperbar = bpb; }
    void set_beatvalue(char bv)     { time_sig.beatvalue = bv; }
    short get_bar() const           { return bar; }
    char get_beatsperbar() const    { return time_sig.beatsperbar; }
    char get_beatvalue() const      { return time_sig.beatvalue; }
    // virtuals from dobj
    stockerrs::ERR_TYPE validate();
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE pt) const;

 private:
    short bar;
    timesig time_sig;
    void init_first();
    
};

#endif
