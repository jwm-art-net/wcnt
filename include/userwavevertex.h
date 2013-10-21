#ifndef USERWAVEVERTEX_H
#define USERWAVEVERTEX_H

#include "dobj.h"

class wave_vertex : public dobj::base
{
public:
    wave_vertex();
    wave_vertex(double udeg, double upos, double ldeg, double lpos);
    ~wave_vertex(){};
    double get_updeg() const {return up_deg;}
    double get_uppos() const {return up_pos;}
    double get_lodeg() const {return lo_deg;}
    double get_lopos() const {return lo_pos;}
    void set_updeg(double n) { up_deg = n;}
    void set_uppos(double n) { up_pos = n;}
    void set_lodeg(double n) { lo_deg = n;}
    void set_lopos(double n) { lo_pos = n;}
    void modulate(double vmod, double hmod);
    virtual errors::TYPE validate();
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE pt) const;
    // oh so naughty:
    double out_deg;
    double out_pos;
private:
    double up_deg;
    double up_pos;
    double lo_deg;
    double lo_pos;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
