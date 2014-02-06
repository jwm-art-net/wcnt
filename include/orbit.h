#ifndef ORBIT_H
#define ORBIT_H

#include "synthmod.h"

class orbit : public synthmod::base
{
 public:
    orbit(const char*);
    ~orbit();
    enum ORBIT_TYPE{
        ORB_HOPALONG,
        ORB_THREEPLY,
        ORB_QUADRUP
    };
    void init(); // performs iteration test if (test_iter > 0)
    void run();
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    const void* get_out(int output_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
    errors::TYPE validate();

 private:
    double out_x;
    double out_y;
    const STATUS * in_restart_trig;
    const STATUS * in_trig;
    ORBIT_TYPE type;
    double const_a;
    double const_b;
    double const_c;
    samp_t test_iter;
    double x;
    double y;
    double scale;
    double cos_b_pc;
    double sin_abc;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
