#include "../include/orbit.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

#include <iostream>
#include <math.h>

orbit::orbit(char const* uname) :
 synthmod(synthmodnames::ORBIT, uname, SM_DEFAULT),
 out_x(0.0), out_y(0.0),
 in_restart_trig(0), in_trig(0), type(ORB_HOPALONG),
 const_a(0.0), const_b(0.0), const_c(0.0),
 test_iter(0),
 x(0.0), y(0.0), scale(1.0), cos_b_pc(0), sin_abc(0)
{
    register_input(inputnames::IN_RESTART_TRIG);
    register_input(inputnames::IN_TRIG);
    register_output(outputnames::OUT_X);
    register_output(outputnames::OUT_Y);
    init_first();
}

orbit::~orbit()
{
}

void const* orbit::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG:
            return in_trig = (STATUS*)o;
        case inputnames::IN_RESTART_TRIG:
            return in_restart_trig = (STATUS*)o;
        default:
            return 0;
    }
}

void const* orbit::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG:           return in_trig;
        case inputnames::IN_RESTART_TRIG:   return in_restart_trig;
        default: return 0;
    }
}

bool orbit::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::ORBIT_TYPE:
        type = *(ORBIT_TYPE*)data;
        return true;
    case paramnames::A:
        const_a = *(double*)data;
        return true;
    case paramnames::B:
        const_b = *(double*)data;
        return true;
    case paramnames::C:
        const_c = *(double*)data;
        return true;
    case paramnames::TEST_ITER:
        test_iter = *(unsigned long*)data;
        return true;
    default:
        return false;
    }
}

void const* orbit::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::ORBIT_TYPE:    return &type;
        case paramnames::A:             return &const_a;
        case paramnames::B:             return &const_b;
        case paramnames::C:             return &const_c;
        case paramnames::TEST_ITER:     return &test_iter;
        default: return 0;
    }
}

void orbit::init()
{
    double max_px = 0, max_py = 0, max_nx = 0, max_ny = 0;
    // set but unused double dfx, dfy;
    // during test, force iteration by using an always-on trigger:
    const STATUS* trig_input = in_trig;
    STATUS trig = ON;
    in_trig = &trig;
    if(test_iter == 0)
        return;
    for(unsigned long i = 0; i < test_iter; i++){
        run();
        if (out_x > 0){ if(out_x > max_px) max_px = out_x; }
        else if (out_x < 0){ if (out_x < max_nx) max_nx = out_x; }
        if (out_y > 0){ if(out_y > max_py) max_py = out_y; }
        else if (out_y < 0){ if (out_y < max_ny) max_ny = out_y; }
    }
    // set but unused dfx = max_px - max_nx;
    // set but unused dfy = max_py - max_ny;
    double max = max_px;
    if(max < -max_nx) max = -max_nx;
    if(max < -max_ny) max = -max_ny;
    if(max < max_py) max = max_py;
    scale = 1 / max;
    if(jwm.is_verbose()){
        std::cout << "\ntest report for orbit synthmod";
        std::cout << "\n------------------------------";
        std::cout << "\nmodule name: " << synthmod::get_username();
        std::cout << "\n after " << test_iter << " iterations...";
        std::cout << "\n x range: " << max_nx << " " << max_px;
        std::cout << "\n y range: " << max_ny << " " << max_py;
        std::cout << "\n---- auto adjustment ----";
        std::cout << "\n scale: " << scale;
    }
    out_x = x = 0;
    out_y = y = 0;
    // restore the trigger to actually be used during run:
    in_trig = trig_input;
    cos_b_pc = cos(const_b) + const_c;
    sin_abc = sin(const_a + const_b + const_c);
}

void orbit::run()
{
    double ox = x, oy = y, fcxb;
    if(*in_restart_trig == ON) {
        out_x = x = 0;
        out_y = y = 0;
    }
    if(*in_trig == ON) {
        switch(type){
            case ORB_THREEPLY:
                x = oy - (ox > 0 ? 1 : (ox < 0 ? -1 : 0)) *
                    fabs(sin(ox) * cos_b_pc - ox * sin_abc);
                y = const_a - ox;
            break;
            case ORB_QUADRUP:
                fcxb = fabs(const_c * ox - const_b);
                x = oy - (ox > 0 ? 1 : (ox < 0 ? -1 : 0)) *
                    sin(fabs(const_b * ox - const_c) *
                    atan(fcxb * fcxb));
                y = const_a - ox;
            break;
            case ORB_HOPALONG:
            default:
                x = oy - (ox > 0 ? 1 : (ox < 0 ? -1 : 0)) *
                    sqrt(fabs(const_b * ox - const_c));
                y = const_a - ox;
        }
        out_x = x * scale;
        out_y = y * scale;
    }
}

void const* orbit::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_X: return &out_x;
        case outputnames::OUT_Y: return &out_y;
        default: return 0;
    }
}

void orbit::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::ORBIT_TYPE, "hopalong/threeply/quadrup");
    register_param(paramnames::A);
    register_param(paramnames::B);
    register_param(paramnames::C);
    register_param(paramnames::TEST_ITER);
}

