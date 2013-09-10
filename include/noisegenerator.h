#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H


#include <cstdlib>

#include "synthmod.h"

class noise_generator : public synthmod
{
public:
    noise_generator(char const*);
    ~noise_generator();
    void run() { output = (float) rand() / (RAND_MAX / 2) - 1; }
    void const* get_out(outputnames::OUT_TYPE) const;

private:
    double output;
};

#endif
