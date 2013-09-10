#ifndef DOBJNAMEDOBJ_H
#define DOBJNAMEDOBJ_H

#include <stdio.h>
#include <string.h>

#include "dobjparamlist.h"


class dobjnamedobj : public dobj
{
public:
    dobjnamedobj();
    ~dobjnamedobj();
    void set_dobjname(const char* name);
    const char* get_dobjname() { return dobjname; }
    // virtuals from dobj
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    char* dobjname;
    void create_params();
    static bool done_params;
};

#endif