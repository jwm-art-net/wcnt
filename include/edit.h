#ifndef EDIT_H
#define EDIT_H

#include "dobj.h"
#include "inputnames.h"
#include "paramnames.h"


// edit
// ----
//
// a dobj which allows the editing of parameters of a data object while also
// unifying both parameter and input editing of a module.

// it supercedes the seperate input_editor and param_editor.

// each instance may only edit a single module or data object, but may contain
// multiple changes to it.

// two strings are stored by user interface:
// 1) name of the target.
// 2) a string containing the edits seperated by space


namespace synthmod { class base; }

class edit : public dobj::base
{
public:
    edit();
    ~edit();
    bool set_target_name(const char*);
    void set_edit_str(const char*);
    const char* get_target_name() const { return targetname; }
    const char* get_edit_str() const { return editstr;}

    // virtuals from dobj
    virtual errors::TYPE validate() { return errors::NO_ERROR; }
    bool        set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    bool        do_actions();

private:
    char* targetname;
    char* editstr;
    void register_ui();
    ui::dobjitem_list* get_ui_items();

    bool do_mod_actions(synthmod::base*);
    bool do_dobj_actions(dobj::base*);

    bool mod_edit_param(synthmod::base*, param::TYPE, const char* );
    bool mod_edit_input(synthmod::base*, input::TYPE, const char*, const char*);
    bool dobj_edit_param(dobj::base*, const char*, const char*);
};

#endif
