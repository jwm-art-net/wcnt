#ifndef UI_MODITEM_H
#define UI_MODITEM_H

#include "checkvalue.h"
#include "listwork.h"
#include "synthmodnames.h"
#include "ui_item.h"

// a mechanism for registering the UI components of a synthmodule.
// it brings together all the UI components into a single class
// rather than the old modparam, modinput, and moddobj classes.
// this will allow a module to intermix inputs, and parameters within
// its UI definition to hopefully make more sense to the user.
//
// beginning to see that the old strict ordering of inputs then params
// was not so bad after all - apart from where it removed a controlling
// parameter from the input it controlled (ie frequency modulation)...
// will have to see how this pans out... perhaps maintain the general
// principle of inputs then params, except for a few cases.


namespace synthmod { class base; };

namespace ui
{
 typedef       base<synthmod::base*, synthmod::TYPE> moditem;
 typedef param_item<synthmod::base*, synthmod::TYPE> modparam;
 typedef input_item<synthmod::base*, synthmod::TYPE> modinput;
 typedef  dobj_item<synthmod::base*, synthmod::TYPE> moddobj;

 /* moditem_list could be a template but the convolution cost didn't...
    (as far as my awesome skills at this moment it time are concerned)
    ... didn't outweigh the cost of code duplication between synthmod
    and dobj.
  */

 class moditem_list : public linked_list<moditem>
 {
  public:
    moditem_list();
    moditem_list(DESTRUCTION);
    ~moditem_list();

    moditem* add_item(synthmod::TYPE, input::TYPE);
    moditem* add_item(synthmod::TYPE, param::TYPE);
    moditem* add_item(synthmod::TYPE, param::TYPE, const char* fixed_str);
    moditem* add_item(synthmod::TYPE, dobj::TYPE parent, dobj::TYPE child);

    moditem* get_first_of_type(synthmod::TYPE smt) {
        result = find_in_data(sneak_first(), search = smt);
        return result->get_data();
    }

    moditem* get_next_of_type() {
        result = find_in_data(result->get_next(), search);
        return result->get_data();
    }

    linkedlist* items_for_module(synthmod::TYPE smt) {
        return new_list_of_by(this, smt);
    }

    bool validate(synthmod::base*, param::TYPE, errors::TYPE);

  private:
    synthmod::TYPE  search;
    llitem*         result;
 };
};

#endif
