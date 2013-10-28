#ifndef UI_MODITEM_H
#define UI_MODITEM_H

#include "ui_item_list.h"

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
 typedef   error_item<synthmod::base*> moderror;
 typedef   param_item<synthmod::base*> modparam;
 typedef   input_item<synthmod::base*> modinput;
 typedef    dobj_item<synthmod::base*> moddobj;
 typedef comment_item<synthmod::base*> modcomment;
 typedef    item_list<synthmod::base*> moditem_list;
};

#endif
