#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include "dobj.h"

// inputedit
// ---------
//
// a dobj which allows module inputs to be reconnected
//
// two strings are stored by user interface, the first being
// taken as the module name, the second as a list of words
// and the words turn out to be alternating between an input name
// name, and the output module and it's output to connect to.

class inputedit : public dobj::base
{
public:
    inputedit();
    ~inputedit();
    bool set_modname(const char*);
    void set_iostr(const char*);
    const char* get_modname() const { return modname; }
    const char* get_iostr() const { return iostr;}
    bool create_connectors();
    // virtuals from dobj
    virtual errors::TYPE validate(){
        return errors::NO_ERROR;
    }
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    char* modname;
    char* iostr;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
