#ifndef UI_DOBJITEM_H
#define UI_DOBJITEM_H

#include "checkvalue.h"
#include "listwork.h"
#include "ui_item.h"

#include <iostream>

namespace dobj { class base; };

namespace ui
{
 typedef         base<dobj::base*> dobjitem;
 typedef   param_item<dobj::base*> dobjparam;
 typedef    dobj_item<dobj::base*> dobjdobj;
 typedef comment_item<dobj::base*> dobjcomment;

 class dobjitem_list : public linked_list<dobjitem>
 {
  public:
    dobjitem_list();
    dobjitem_list(DESTRUCTION);
    ~dobjitem_list();

    dobjitem* add_item(param::TYPE);
    dobjitem* add_item(param::TYPE, const char* fixed_str);
    dobjitem* add_item(dobj::TYPE parent, dobj::TYPE child);
    dobjitem* add_item(const char* comment);

    bool validate(dobj::base*, param::TYPE, errors::TYPE);

  private:
    FLAGS skip_id;
    FLAGS match_id;
 };
};

#endif
