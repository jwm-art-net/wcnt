#ifndef MODINPUT_H
#define MODINPUT_H

#include "inputnames.h"
#include "synthmodnames.h"

class synthmod;

class modinput
{
 public:
    modinput(synthmod*, inputnames::IN_TYPE);
    ~modinput();

    synthmod* get_synthmodule() const { return synthmodule; }

    synthmodnames::SYNTHMOD_TYPE get_moduletype() const;
    inputnames::IN_TYPE get_inputtype() const;
    #ifdef UNUSED
    iocat::IOCAT get_inputcategory() const;
    #endif

    bool operator()(synthmod* sm) const {
        return synthmodule == sm;
    }

    bool operator()(inputnames::IN_TYPE & it) const {
        return input_type == it;
    }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod* synthmodule;
    inputnames::IN_TYPE input_type;

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};

#endif
