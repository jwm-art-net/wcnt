#ifndef CONSTANT_NOTE_H
#define CONSTANT_NOTE_H

#include "synthmod.h"
#include "jwm_init.h"

class constant_note : public synthmod
{
public:
    constant_note(char const*);
    ~constant_note();
    void run(){};
    void init();
    void const* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    stockerrs::ERR_TYPE validate();

private:
    char* note; /* is param and output */
    double out_freq;
    double out_phase_step;
    void set_note(const char*);
    void create_params();
    
};

#endif
