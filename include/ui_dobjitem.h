#ifndef UI_DOBJITEM_H
#define UI_DOBJITEM_H

#include "ui_item_list.h"


namespace dobj { class base; };

namespace ui
{
 typedef         base<dobj::base*> dobjitem;
 typedef   error_item<dobj::base*> dobjerror;
 typedef   param_item<dobj::base*> dobjparam;
 typedef   input_item<dobj::base*> dobjinput;
 typedef    dobj_item<dobj::base*> dobjdobj;
 typedef comment_item<dobj::base*> dobjcomment;
 typedef    item_list<dobj::base*> dobjitem_list;
};

#endif
