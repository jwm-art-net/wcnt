#ifndef STEPDATA_H
#define STEPDATA_H

#include "dobj.h"

/*
    step_data
    ---------

    are inserted into the stepper module step list.
    positions are purely in the range of 0.0 to 1.0, levels can be
    anything.

    the stepper module creates an array of size N (its stepcount) to
    stretch from positions 0.0 to 1.0 and computes both the levels for
    each index, and it outputs one index continually until it is triggered
    to step to the next.
*/

class step_data : public dobj
{
public:
    step_data();
    step_data(double pos, double upperlevel, double lowerlevel);
    ~step_data();

    void set_position(double p) { position = p; }
    void set_upper_level(double ul) { up_level = ul; }
    void set_lower_level(double ll) { lo_level = ll; }

    double get_position() { return position; }
    double get_upper_level() { return up_level; }
    double get_lower_level() { return lo_level; }

    // virtuals from dobj
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE) const;
    stockerrs::ERR_TYPE validate();

private:
    double position;
    double up_level;
    double lo_level;
    void create_params();
    static bool done_params;
};

#endif
