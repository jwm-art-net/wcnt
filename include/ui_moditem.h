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
 typedef         base<synthmod::base*> moditem;
 typedef   param_item<synthmod::base*> modparam;
 typedef   input_item<synthmod::base*> modinput;
 typedef    dobj_item<synthmod::base*> moddobj;
 typedef comment_item<synthmod::base*> modcomment;

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

    moditem* add_item(input::TYPE);
    moditem* add_item(param::TYPE);
    moditem* add_item(param::TYPE, const char* fixed_str);
    moditem* add_item(dobj::TYPE parent, dobj::TYPE child);
    moditem* add_item(const char* comment);

    bool validate(synthmod::base*, param::TYPE, errors::TYPE);

  private:
    FLAGS skip_id;
    FLAGS match_id;
 };
};

#endif
