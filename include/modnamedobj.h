#ifndef MODNAMEDOBJ_H
#define MODNAMEDOBJ_H

#include "dobj.h"

class modnamedobj : public dobj::base
{
public:
    modnamedobj();
    ~modnamedobj();
    void set_modname(const char* name);
    const char* get_modname() const { return modname; }
    // virtuals from dobj
    virtual errors::TYPE validate() {
        return errors::NO_ERROR;
    }
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    char* modname;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
