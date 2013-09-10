#ifndef MODOUTPUT_H
#define MODOUTPUT_H

#include "synthmodnames.h"
#include "outputnames.h"

class synthmod;

class modoutput
{
 public:
    modoutput(synthmod*, outputnames::OUT_TYPE);
    ~modoutput(){};

    synthmod* get_synthmodule() { return synthmodule; }

    synthmodnames::SYNTH_MOD_TYPE get_moduletype();
    outputnames::OUT_TYPE get_outputtype();
    iocat::IOCAT get_outputcategory();

    bool operator()(synthmod* sm) const {
        return synthmodule == sm;
    }

    bool operator()(outputnames::OUT_TYPE & ot) const {
        return output_type == ot;
    }

 private:
    synthmod* synthmodule;
    outputnames::OUT_TYPE output_type;
};

#endif
