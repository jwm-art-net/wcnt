#include "../include/modpart.h"

#include "../include/synthmod.h"

namespace modpart
{

 base::base(synthmod::base* _sm) :
  sm(_sm), flags(0)
 {
 }

 const void* base::set_in(int, const void*)
 {
    return 0;
 }

 const void* base::get_in(int) const
 {
    return 0;
 }

 bool base::set_param(int, const void*)
 {
    return 0;
 }

 const void* base::get_param(int) const
 {
    return 0;
 }

 const void* base::get_out(int) const
 {
    return 0;
 }

 base* base::duplicate_modpart()
 {
    return 0;
 }

 bool base::sm_done_first()
 {
    return sm->type_registered();
 }

 ui::moditem* base::register_sm_param(int pt)
 {
    return sm->register_param(pt);
 }

 ui::moditem* base::register_sm_param(int pt, const char* fixed_string)
 {
    return sm->register_param(pt, fixed_string);
 }

 ui::moditem* base::register_sm_input(int it)
 {
    return sm->register_input(it);
 }

 bool base::validate_sm_param(int pt, errors::TYPE et)
 {
    return sm->validate_param(pt, et);
 }
}; // namespace modpart
