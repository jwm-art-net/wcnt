#ifndef METERCHANGE_H
#define METERCHANGE_H

#include "dobj.h"

// struct timesig used by synthfilereader to read time signature in one
// go, so user can represent time signature as 4/4 or 3/4 etc rather than
// as two parts such as beatsperbar 3 beatvalue 4

struct timesig
{
    wcint_t beatsperbar;
    wcint_t beatvalue;
};

class meterchange : public dobj::base
{
 public:
    meterchange();
    meterchange(wcint_t br, char btpb, char btval);
    // these accessor methods are used by timemap
    void set_bar(wcint_t br)          { bar = br; }
    void set_beatsperbar(char bpb)  { time_sig.beatsperbar = bpb; }
    void set_beatvalue(char bv)     { time_sig.beatvalue = bv; }
    wcint_t get_bar() const           { return bar; }
    char get_beatsperbar() const    { return time_sig.beatsperbar; }
    char get_beatvalue() const      { return time_sig.beatvalue; }
    // virtuals from dobj
    errors::TYPE validate();
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

 private:
    wcint_t bar;
    timesig time_sig;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
