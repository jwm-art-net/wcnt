#ifndef DYNVERTEX_H
#define DYNVERTEX_H

#include "dobj.h"

/*
dynvertex - dynamic vertex

dynvertex stores data for a vertex in the dynamic module.  The module
can hold any number of dynvertex.  They are used for applying amplitude
levels to an input signal, dependant on the level of that signal.

si_level - signal in level
    amplitude of input signal which the vertex refers to

uso_level - upper signal out level
    stores amplitude of output signal which corresponds to a
    height of 1.0

lso_level - lower signal out level
    stores amplitude of output signal which corresponds to a
    height of 0.0

get_out_level(height)
    returns the output signal level for height.  
    *** height MUST be in range of 0.0 to 1.0 ***

*/
class dynvertex : public dobj
{
 public:
    dynvertex();
    dynvertex(double sil, double usol, double lsol);
    ~dynvertex(){};
    void set_signal_in_level(double sil)        { si_level = sil;}
    void set_upper_signal_out_level(double usol){ uso_level = usol;}
    void set_lower_signal_out_level(double lsol){ lso_level = lsol;}
    double get_signal_in_level() const          { return si_level;}
    double get_upper_signal_out_level() const   { return uso_level;}
    double get_lower_signal_out_level() const   { return lso_level;}
    double get_out_level(double height) const {
        return lso_level * (1 - height) + uso_level * height; }
    stockerrs::ERR_TYPE validate();
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE pt) const;

 private:
    double si_level;
    double uso_level;
    double lso_level;
    void init_first();
};

#endif
