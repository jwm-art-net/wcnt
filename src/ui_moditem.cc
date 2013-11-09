#include "../include/ui_moditem.h"

#include "../include/globals.h"


namespace ui
{
 template<>
 char item_list<synthmod::base*>::err_msg[STRBUFLEN] = { '\0' };

 template<>
 const char* error_item<synthmod::base*>::err_msg = 0;


 template <>
 base<synthmod::base*>* item_list<synthmod::base*>::add_item(input::TYPE it)
 {
    if (!goodlist(LIST_STD))
        return 0;

    base<synthmod::base*>* i = new input_item<synthmod::base*>(it);
    if (i) {
        if (!linked_list< base<synthmod::base*> >::add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    return i;
 }


 template<>
 connector* item_list<synthmod::base*>::add_connector_off(synthmod::base* sm,
                                                          input::TYPE it)
 {
    return wcnt::get_connectlist()->add_connector_off(sm, it);
 }

}; //namespace ui
