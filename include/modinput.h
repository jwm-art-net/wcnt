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

    synthmod* get_synthmodule() const { return synthmodule; }

    synthmodnames::SYNTH_MOD_TYPE get_moduletype() const;
    inputnames::IN_TYPE get_inputtype() const;
    iocat::IOCAT get_inputcategory() const;

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
