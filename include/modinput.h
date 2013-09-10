#ifndef MODINPUT_H
#define MODINPUT_H

#ifndef BARE_MODULES

#include "synthmodule.h"

class modinput
{
public:
    modinput(synthmod*, inputnames::IN_TYPE);
    ~modinput(){};
    synthmod* get_synthmodule() {
        return (this != NULL) ? synthmodule : NULL; 
    }
    synthmodnames::SYNTH_MOD_TYPE get_moduletype();
    inputnames::IN_TYPE get_inputtype();
    IOCAT get_inputcategory();

private:
    synthmod* synthmodule;
    inputnames::IN_TYPE input_type;
};

#endif
#endif
