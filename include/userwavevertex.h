#ifndef USERWAVEVERTEX_H
#define USERWAVEVERTEX_H

#include "dobjparamlist.h"

class wave_vertex : public dobj
{
public:
    wave_vertex();
    wave_vertex(double udeg, double upos, double ldeg, double lpos);
    ~wave_vertex(){};
    double get_updeg(){return up_deg;}
    double get_uppos(){return up_pos;}
    double get_lodeg(){return lo_deg;}
    double get_lopos(){return lo_pos;}
    void set_updeg(double n){ up_deg = n;}
    void set_uppos(double n){ up_pos = n;}
    void set_lodeg(double n){ lo_deg = n;}
    void set_lopos(double n){ lo_pos = n;}
    void modulate(double vmod, double hmod);
#ifndef BARE_MODULES
    virtual stockerrs::ERR_TYPE validate();
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE pt);
#endif
    // oh so naughty:
    double out_deg;
    double out_pos;
private:
    double up_deg;
    double up_pos;
    double lo_deg;
    double lo_pos;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
