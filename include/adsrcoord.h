#ifndef ADSRCOORD_H
#define ADSRCOORD_H

#include "dobj.h"

class adsr_coord : public dobj::base
{
public:
    enum SECT
    {
        ADSR_ATTACK,
        ADSR_DECAY,
        ADSR_SUSTAIN,
        ADSR_RELEASE
    };
    adsr_coord();
    adsr_coord(SECT s, double ut, double ul, double lt, double ll);
    ~adsr_coord();
    // i do believe this accessor methods are used by the adsr_scaler:
    void set_upper_time(double t)   { upper_time = t; }
    void set_lower_time(double t)   { lower_time = t; }
    void set_upper_level(double l)  { upper_level = l; }
    void set_lower_level(double l)  { lower_level = l; }
    void set_adsr_section(SECT s)   { sect = s; }

    double get_upper_time() const   { return upper_time; }
    double get_lower_time() const   { return lower_time; }
    double get_upper_level() const  { return upper_level; }
    double get_lower_level() const  { return lower_level; }
    SECT get_adsr_section() const   { return sect; }

    void run(double velocity);
    double output_time;
    double output_level;
    // virtuals from dobj
    errors::TYPE validate();
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE pt) const;

    bool operator()(SECT & s) const {
        return this->sect == s;
    }

    bool operator()(int & s) const {
        return (int)this->sect == s;
    }

private:
    SECT sect;
    double upper_time;
    double upper_level;
    double lower_time;
    double lower_level;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
