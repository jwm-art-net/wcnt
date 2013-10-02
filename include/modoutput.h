#ifndef MODOUTPUT_H
#define MODOUTPUT_H

#include "outputnames.h"
#include "synthmodnames.h"
#include "types.h"


class synthmod;

class modoutput
{
 public:
    modoutput(synthmod*, output::TYPE);
    ~modoutput();

    synthmod* get_synthmodule() const { return module; }

    module::TYPE    get_moduletype() const;
    output::TYPE    get_outputtype() const;
    iocat::TYPE     get_outputcategory() const;

    bool operator()(synthmod* sm) const {
        return module == sm;
    }

    bool operator()(output::TYPE & ot) const {
        return output_type == ot;
    }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod*       module;
    output::TYPE    output_type;

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};

#endif
