#ifndef MODNAMEDOBJ_H
#define MODNAMEDOBJ_H

#include "dobj.h"

class modnamedobj : public dobj
{
public:
    modnamedobj();
    ~modnamedobj();
    void set_modname(const char* name);
    const char* get_modname() const { return modname; }
    // virtuals from dobj
    virtual stockerrs::ERR_TYPE validate() {
        return stockerrs::ERR_NO_ERROR;
    }
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    char* modname;
    void init_first();
    
};

#endif
