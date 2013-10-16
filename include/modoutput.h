#ifndef MODOUTPUT_H
#define MODOUTPUT_H

#include "synthmod.h"



class modoutput
{
 public:
    modoutput(synthmod::base*, output::TYPE);
    ~modoutput();

    synthmod::base* get_module() const { return module; }

    synthmod::TYPE  get_moduletype() const;
    output::TYPE    get_outputtype() const;
    iocat::TYPE     get_outputcategory() const;

    bool operator()(synthmod::base* sm) const { return module == sm; }
    bool operator()(output::TYPE & ot) const { return output_type == ot; }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod::base* module;
    output::TYPE    output_type;

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};

#endif
