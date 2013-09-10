#ifndef DYNVERTEX_H
#define DYNVERTEX_H

#include "dobjparamlist.h"
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
    void set_signal_in_level(double sil){ si_level = sil;}
    void set_upper_signal_out_level(double usol){ uso_level = usol;}
    void set_lower_signal_out_level(double lsol){ lso_level = lsol;}
    double get_signal_in_level(){ return si_level;}
    double get_upper_signal_out_level(){ return uso_level;}
    double get_lower_signal_out_level(){ return lso_level;}
    double get_out_level(double height) {
        return lso_level * (1 - height) + uso_level * height; }
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE pt);
#endif
private:
    double si_level;
    double uso_level;
    double lso_level;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
