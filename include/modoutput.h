#ifndef MODOUTPUT_H
#define MODOUTPUT_H

#include "synthmod.h"



class modoutput
{
 public:
    modoutput(synthmod::base*, int out_type);
    ~modoutput();

    synthmod::base* get_module() const { return module; }

    synthmod::TYPE  get_moduletype() const;
    int             get_outputtype() const;
    iocat::TYPE     get_outputcategory() const;

    bool operator()(synthmod::base* sm) const { return module == sm; }
    bool operator()(int & ot) const { return output_type == ot; }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod::base* module;
    int             output_type;

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};

#endif
