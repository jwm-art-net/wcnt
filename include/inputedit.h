#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include "dobj.h"

// inputedit
// ---------
//
// a dobj which allows module inputs to be reconnected to a different output
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
    const char* get_modname() const { return modname; }
    bool create_connectors();
    // virtuals from dobj
    virtual errors::TYPE validate(){
        return errors::NO_ERROR;
    }
    bool        set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    char* modname;
    char* iostr;
    void register_ui();
    ui::dobjitem_list* get_ui_items();

    bool set_modname(const char*);
    void set_iostr(const char*);
    const char* get_iostr() const { return iostr;}
};

#endif
