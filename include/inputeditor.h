#ifndef INPUTEDITOR_H
#define INPUTEDITOR_H

#include "inputedit.h"
#include "linkedlist.h"

class inputeditor : public dobj::base, public linked_list<inputedit>
{
 public:
    inputeditor();
    ~inputeditor();

    /*
    // creates the connectors from the inputedits.
    */
    bool create_connectors();

    /*
    // virtuals from dobj
    */
    virtual errors::TYPE validate(){
        return errors::NO_ERROR;
    }

    const dobj::base* add_dobj(dobj::base*);

 private:
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
