#ifndef MODINPUT_H
#define MODINPUT_H

#include "inputnames.h"
#include "synthmodnames.h"
#include "types.h"


class synthmod;

class modinput
{
 public:
    modinput(synthmod*, input::TYPE);
    ~modinput();

    synthmod*       get_synthmodule() const { return synthmodule; }
    module::TYPE    get_moduletype() const;
    input::TYPE     get_inputtype() const;
    #ifdef UNUSED
    iocat::TYPE     get_inputcategory() const;
    #endif

    bool operator()(synthmod* sm) const {
        return synthmodule == sm;
    }

    bool operator()(input::TYPE & it) const {
        return input_type == it;
    }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod*   synthmodule;
    input::TYPE input_type;

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};

#endif
