#ifndef DOBJNAMEDOBJ_H
#define DOBJNAMEDOBJ_H

#include "dobj.h"

// A data object for holding the username of a named data object.
// named data objects are standalone data objects of themselves, not
// something contained by another data object or module.
// ie riff, wavfilein, group

// however some modules and data objects require a list of named data objects.
// dobjnamedobj is used for passing a named data object to them by name only.

class dobjnamedobj : public dobj::base
{
public:
    dobjnamedobj();
    ~dobjnamedobj();
    void set_dobjname(const char* name);
    const char* get_dobjname() const { return dobjname; }
    // virtuals from dobj
    virtual errors::TYPE validate(){
        return errors::NO_ERROR;
    }
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    char* dobjname;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
