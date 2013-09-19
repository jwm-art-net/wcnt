#ifndef BPMCHANGE_H
#define BPMCHANGE_H

#include "dobj.h"

// tempo changes are now relative.
// so when tobpm is 10, the tempo will increase by 10 bpm.
// obvious: negative tempo decrease speed

class bpmchange : public dobj
{
public:
    bpmchange();
    bpmchange(short bar, double bpm);
    void set_bar(short bar){atbar = bar;}
    void set_bpm(double bpm){tobpm = bpm;}
    short get_bar() const { return atbar;}
    double get_bpm() const { return tobpm;}
    stockerrs::ERR_TYPE validate();
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE pt) const;

private:
    short atbar;
    double tobpm;
    void init_first();
};

#endif
