#ifndef TIMING_H
#define TIMING_H

#include "dobj.h"

// a length of time

class timing : public dobj
{
public:
    timing();
    timing(double s);
    void set_time(double s){seconds = s;}
    double get_time(){ return seconds;}
    stockerrs::ERR_TYPE validate();
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE pt) const;

private:
    double seconds;
    void create_params();
    static bool done_params;
};

#endif
