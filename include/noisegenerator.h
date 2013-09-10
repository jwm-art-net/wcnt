#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include <stdlib.h>
#include <time.h>
#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modinputslist.h"
#include "modoutputslist.h"
#include "modparamlist.h"
#endif

class noise_generator : public synthmod
{
public:
    noise_generator(char const*);
    ~noise_generator();
    double const* get_output_signal();
    // virtual funcs
    void run() { output = (float) rand() / (RAND_MAX / 2) - 1; }
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
#endif
private:
    double output;
    static int noise_generator_count;
};

#endif
