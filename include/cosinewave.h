#ifndef COSINEWAVE_H
#define COSINEWAVE_H

#include "synthmodule.h"

/*    cosine_wave
---------------

input_phase_trig    - trigger  generation of cycles
input_phase_step      - how many degree's to step through at each sample
                      (to generate cosine of frequency specified by clock)
recycle_mode        - ON: to never stop generating
                      OFF: stop after 360 * cycles
on_trig_zero_deg    - ON:  reset phase on phase_trig  (imperfect), 
                      OFF:  perfection in normal circumstances
cycles              - number of cycles to generate on phase trig

notes
-----

for oscilators:
---------------
for normal expected behaviour, phase_trig & phase_step should be
from the same oscclock.  recycle mode = OFF, on_trig_zero_degs = OFF
cycles = 1.00

for other behaviour for example to generate one cycle at
octave 1 triggered from oscclock at octave -1, set recycle mode = OFF,
cycles = 1.
to generate half a cycle at octave -1 triggered from oscclock
at octave 0 set on_trig_zero_degs = ON 

for LFO's
---------
set phase trig and degsize from same lfoclock. or whatever...
See LFO documentation.                                      */

class cosine_wave : public synthmod
{
public:
    cosine_wave(char const*);
    ~cosine_wave();
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    double output;
    STATUS play_state;
    const STATUS * in_phase_trig;
    const double * in_phase_step;
    double degs;
    STATUS recycle;
    STATUS reset_phase;
    double cycles;
    double maxdegs;
    void create_params();
    static bool done_params;
};

#endif
