#ifndef DOBJNAMEDOBJ_H
#define DOBJNAMEDOBJ_H

#include "dobj.h"

class dobjnamedobj : public dobj
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
    void init_first();
    
};

#endif
