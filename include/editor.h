#ifndef EDITOR_H
#define EDITOR_H

#include "edit.h"
#include "linkedlist.h"

class editor : public dobj::base, public linked_list<edit>
{
 public:
    editor();
    ~editor();

    // do_actions processes the edit data (previously stored as strings by
    // synthfilereader).
    bool do_actions();

    // virtuals from dobj
    virtual errors::TYPE validate() { return errors::NO_ERROR; };

    const dobj::base* add_dobj(dobj::base*);

 private:
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
