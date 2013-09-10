#ifndef MODINPUT_H
#define MODINPUT_H

#include "inputnames.h"
#include "synthmodnames.h"

class synthmod;

class modinput
{
 public:
    modinput(synthmod*, inputnames::IN_TYPE);
    ~modinput(){};

    synthmod* get_synthmodule() { return synthmodule; }

    synthmodnames::SYNTH_MOD_TYPE get_moduletype();
    inputnames::IN_TYPE get_inputtype();
    iocat::IOCAT get_inputcategory();

    bool operator()(synthmod* sm) const {
        return synthmodule == sm;
    }

    bool operator()(inputnames::IN_TYPE & it) const {
        return input_type == it;
    }

private:
    synthmod* synthmodule;
    inputnames::IN_TYPE input_type;
};

#endif
