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
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    char* modname;
    void register_ui();
};

#endif
