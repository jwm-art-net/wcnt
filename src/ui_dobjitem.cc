#include "../include/ui_dobjitem.h"

namespace ui
{
 template<>
 char item_list<dobj::base*>::err_msg[STRBUFLEN] = { '\0' };

 template<>
 const char* error_item<dobj::base*>::err_msg = 0;


 template <>
 input_item<dobj::base*>* item_list<dobj::base*>::add_input_item(int)
 {
    return 0;
 }


 template<>
 connector* item_list<dobj::base*>::add_connector_off(dobj::base*, int)
 {
    return 0;
 }

 template<>
 connector* item_list<dobj::base*>::add_connector_self(dobj::base*, int, int)
 {
    return 0;
 }


 template<>
 connector* item_list<dobj::base*>::add_connector_as(dobj::base*, int, int)
 {
    return 0;
 }

}; //namespace ui
