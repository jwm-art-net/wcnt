#ifndef BPMCHANGE_H
#define BPMCHANGE_H

#include "dobjparamlist.h"

// by the time <atbar> is reached bpm will be <tobpm>
// (sigh of relief) this one's nice and easy......(adding dobj)

class bpmchange : public dobj
{
public:
    bpmchange();
    bpmchange(short bar, double bpm);
    void set_bar(short bar){atbar = bar;}
    void set_bpm(double bpm){tobpm = bpm;}
    short get_bar(){ return atbar;}
    double get_bpm(){ return tobpm;}
#ifndef BARE_MODULES
    stockerrs::ERR_TYPE validate();
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE pt);
#endif
private:
    short atbar;
    double tobpm;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
