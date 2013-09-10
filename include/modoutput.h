#ifndef MODOUTPUT_H
#define MODOUTPUT_H

#include "synthmodule.h"

class modoutput
{
  public:
    modoutput(synthmod *, outputnames::OUT_TYPE);
    ~modoutput(){};
    synthmod *getsynthmodule(){return (this != NULL) ? synthmodule : NULL;}
    synthmodnames::SYNTH_MOD_TYPE getmoduletype();
    outputnames::OUT_TYPE getoutputtype();
    IOCAT getoutputcategory();
  private:
    synthmod * synthmodule;
    outputnames::OUT_TYPE output_type;
};

#endif
