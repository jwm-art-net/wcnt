#ifndef DOBJNAMEDOBJ_H
#define DOBJNAMEDOBJ_H

#include "dobj.h"

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
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    char* dobjname;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
