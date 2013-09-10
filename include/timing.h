#ifndef TIMING_H
#define TIMING_H

#include "dobjparamlist.h"

// a length of time

class timing : public dobj
{
public:
    timing();
    timing(double s);
    void set_time(double s){seconds = s;}
    double get_time(){ return seconds;}
#ifndef BARE_MODULES
    stockerrs::ERR_TYPE validate();
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE pt);
#endif
private:
    double seconds;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
