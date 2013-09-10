#ifndef MODOUTPUT_H
#define MODOUTPUT_H

#ifndef BARE_MODULES

#include "synthmodule.h"

class modoutput
{
public:
    modoutput(synthmod *, outputnames::OUT_TYPE);
    ~modoutput(){};
    synthmod* get_synthmodule() {
        return (this != NULL) ? synthmodule : NULL;
    }
    synthmodnames::SYNTH_MOD_TYPE get_moduletype();
    outputnames::OUT_TYPE get_outputtype();
    IOCAT get_outputcategory();
private:
    synthmod * synthmodule;
    outputnames::OUT_TYPE output_type;
};

#endif
#endif
