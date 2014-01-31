#include "../include/ui_moditem.h"

#include "../include/globals.h"


namespace ui
{
 template<>
 char item_list<synthmod::base*>::err_msg[STRBUFLEN] = { '\0' };

 template<>
 const char* error_item<synthmod::base*>::err_msg = 0;

 template <>
 input_item<synthmod::base*>* item_list<synthmod::base*>::add_input_item(input::TYPE it)
 {
    if (!goodlist(LIST_STD))
        return 0;

    input_item<synthmod::base*>* i = new input_item<synthmod::base*>(it);
    if (i) {
        if (!linked_list< base<synthmod::base*> >::add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }

    #ifdef DEBUG
    i->dump();
    #endif

    return i;
 }

 template<>
 connector* item_list<synthmod::base*>::add_connector_off(synthmod::base* sm,
                                                          input::TYPE it)
 {
    return wcnt::get_connectlist()->add_connector_off(sm, it);
 }

 template<>
 connector* item_list<synthmod::base*>::add_connector_self(synthmod::base* sm,
                                                          input::TYPE it,
                                                          output::TYPE ot)
 {
    return wcnt::get_connectlist()->add_connector_self(sm, it, ot);
 }

};
