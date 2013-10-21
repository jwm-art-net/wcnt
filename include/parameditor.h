#ifndef PARAMEDITOR_H
#define PARAMEDITOR_H

#include "paramedit.h"
#include "linkedlist.h"

class parameditor : public dobj::base, public linked_list<paramedit>
{
 public:
    parameditor();

    bool do_param_edits();

    /*
    // virtuals from dobj
    */
    virtual errors::TYPE validate(){
        return errors::NO_ERROR;
    }

    const dobj::base* add_dobj(dobj::base* dbj);

 private:
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
