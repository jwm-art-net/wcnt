#ifndef ORBIT_H
#define ORBIT_H

#include "synthmod.h"

class orbit : public synthmod
{
 public:
    orbit(char const*);
    ~orbit();
    enum ORBIT_TYPE{
        ORB_HOPALONG,
        ORB_THREEPLY,
        ORB_QUADRUP
    };
    void init(); // performs iteration test if (test_iter > 0)
    void run();
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    void const* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
 private:
    double out_x;
    double out_y;
    const STATUS * in_restart_trig;
    const STATUS * in_trig;
    ORBIT_TYPE type;
    double const_a;
    double const_b;
    double const_c;
    unsigned long test_iter;
    double x;
    double y;
    double scale;
    double cos_b_pc;
    double sin_abc;
    void create_params();
    
};

#endif
