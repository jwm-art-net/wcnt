#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H


#include <cstdlib>

#include "synthmod.h"

class noise_generator : public synthmod::base
{
public:
    noise_generator(const char*);
    ~noise_generator();
    void run() { output = (double) rand() / (RAND_MAX / 2.0) - 1.0; }
    const void* get_out(output::TYPE) const;

private:
    double output;
    void register_ui() {};
    ui::moditem_list* get_ui_items() { return 0;}
};

#endif
