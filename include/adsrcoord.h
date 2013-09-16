#ifndef ADSRCOORD_H
#define ADSRCOORD_H

#include "dobj.h"

class adsr_coord : public dobj
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
    void set_upper_time(double t){ upper_time = t; }
    void set_lower_time(double t){ lower_time = t; }
    void set_upper_level(double l){ upper_level = l; }
    void set_lower_level(double l){ lower_level = l; }
    void set_adsr_section(SECT s){ sect = s; }
    double get_upper_time(){ return upper_time; }
    double get_lower_time(){ return lower_time; }
    double get_upper_level(){ return upper_level; }
    double get_lower_level(){ return lower_level; }
    SECT get_adsr_section(){ return sect; }
    void run(double velocity);
    double output_time;
    double output_level;
    // virtuals from dobj
    stockerrs::ERR_TYPE validate();
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE pt) const;

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
    void create_params();
    
};

#endif
