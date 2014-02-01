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
    debug("\\________ adding connector 'off' to input '%s' '%s'\n", sm->get_username(),
                                                        input::names::get(it));
    return wcnt::get_connectlist()->add_connector_off(sm, it);
 }

 template<>
 connector* item_list<synthmod::base*>::add_connector_self(synthmod::base* sm,
                                                        input::TYPE it,
                                                        output::TYPE ot)
 {
    debug("\\________ adding connector 'self' to input '%s' '%s' '%s'\n", sm->get_username(),
                                                        input::names::get(it),
                                                        output::names::get(ot));
    return wcnt::get_connectlist()->add_connector_self(sm, it, ot);
 }

 template<>
 connector* item_list<synthmod::base*>::add_connector_as(synthmod::base* sm,
                                                        input::TYPE it,
                                                        input::TYPE asit)
 {
    debug("\\________ adding connector 'as' to input '%s' '%s' '%s'\n", sm->get_username(),
                                                        input::names::get(it),
                                                        input::names::get(asit));
    return wcnt::get_connectlist()->add_connector_as(sm, it, asit);
 }

};
