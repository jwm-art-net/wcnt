#include "../include/ui_moditem.h"

#include "../include/globals.h"


namespace ui
{
 template<>
 char item_list<synthmod::base*>::err_msg[STRBUFLEN] = { '\0' };

 template<>
 const char* error_item<synthmod::base*>::err_msg = 0;

 template<>
 connector* item_list<synthmod::base*>::add_connector_off(synthmod::base* sm,
                                                          input::TYPE it)
 {
    return wcnt::get_connectlist()->add_connector_off(sm, it);
 }

}; //namespace ui
