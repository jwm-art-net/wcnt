#ifndef UI_DOBJITEM_H
#define UI_DOBJITEM_H

#include "checkvalue.h"
#include "listwork.h"
#include "ui_item.h"

#include <iostream>

namespace dobj { class base; };

namespace ui
{
 typedef       base<dobj::base*, dobj::TYPE> dobjitem;
 typedef param_item<dobj::base*, dobj::TYPE> dobjparam;
 typedef  dobj_item<dobj::base*, dobj::TYPE> dobjdobj;
 typedef comment_item<dobj::base*, dobj::TYPE> dobjcomment;

 class dobjitem_list : public linked_list<dobjitem>
 {
  public:
    dobjitem_list();
    dobjitem_list(DESTRUCTION);
    ~dobjitem_list();

    dobjitem* add_item(dobj::TYPE, param::TYPE);
    dobjitem* add_item(dobj::TYPE, param::TYPE, const char* fixed_str);
    dobjitem* add_item(dobj::TYPE, dobj::TYPE parent, dobj::TYPE child);
    dobjitem* add_item(dobj::TYPE, const char* comment);

    dobjitem* get_first_of_type(dobj::TYPE dt) {
        result = find_in_data(sneak_first(), search = dt);
        return result->get_data();
    }

    dobjitem* get_next_of_type() {
        result = find_in_data(result->get_next(), search);
        return result->get_data();
    }

    linkedlist* items_for_data_object(dobj::TYPE dt) {
        return new_list_of_by(this, dt);
    }

    bool validate(dobj::base*, param::TYPE, errors::TYPE);

  private:
    dobj::TYPE  search;
    llitem*     result;
 };
};

#endif
