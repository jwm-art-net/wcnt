#ifndef USERWAVEVERTEX_H
#define USERWAVEVERTEX_H

#include "dobj.h"

class wave_vertex : public dobj::base
{
public:
    wave_vertex();
    wave_vertex(const wave_vertex*);
    wave_vertex(double udeg, double uplvl, double lodeg, double lolvl);
    ~wave_vertex(){};

    void modulate(double vmod, double hmod);
    virtual errors::TYPE validate();
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
    // accessor required for sorted insert
    double get_lodeg() const { return lo_deg; }
    // oh so naughty:
    double out_deg;
    double out_lvl;
private:
    double up_deg;
    double up_lvl;
    double lo_deg;
    double lo_lvl;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
