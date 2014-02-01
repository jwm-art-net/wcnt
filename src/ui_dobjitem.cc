#include "../include/ui_dobjitem.h"

namespace ui
{
 template<>
 char item_list<dobj::base*>::err_msg[STRBUFLEN] = { '\0' };

 template<>
 const char* error_item<dobj::base*>::err_msg = 0;


 template <>
 input_item<dobj::base*>* item_list<dobj::base*>::add_input_item(input::TYPE)
 {
    return 0;
 }


 template<>
 connector* item_list<dobj::base*>::add_connector_off(dobj::base*, input::TYPE)
 {
    return 0;
 }

 template<>
 connector* item_list<dobj::base*>::add_connector_self(dobj::base*, input::TYPE,
                                                                    output::TYPE)
 {
    return 0;
 }


 template<>
 connector* item_list<dobj::base*>::add_connector_as(dobj::base*, input::TYPE,
                                                                  input::TYPE)
 {
    return 0;
 }

}; //namespace ui
