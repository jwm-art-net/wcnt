#ifndef MODOUTPUT_H
#define MODOUTPUT_H

#include "synthmodnames.h"
#include "outputnames.h"

class synthmod;

class modoutput
{
 public:
    modoutput(synthmod*, outputnames::OUT_TYPE);
    ~modoutput();

    synthmod* get_synthmodule() const { return synthmodule; }

    synthmodnames::SYNTH_MOD_TYPE get_moduletype() const;
    outputnames::OUT_TYPE get_outputtype() const;
    iocat::IOCAT get_outputcategory() const;

    bool operator()(synthmod* sm) const {
        return synthmodule == sm;
    }

    bool operator()(outputnames::OUT_TYPE & ot) const {
        return output_type == ot;
    }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod* synthmodule;
    outputnames::OUT_TYPE output_type;

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};

#endif
