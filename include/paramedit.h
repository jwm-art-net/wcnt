#ifndef PARAMEDIT_H
#define PARAMEDIT_H

#include "dobj.h"

/*
//  paramedit
// -----------
// two strings are stored by user interface, the first being
// taken as the module name, the second as a list of words
// and the words turn out to be alternating between a parameter
// name and it's value.
*/

namespace synthmod { class base; }

class paramedit : public dobj::base
{
public:
    paramedit();
    ~paramedit();
    bool        set_name(const char*);
    void        set_parstr(const char*);
    const char* get_name() const { return name; }
    const char* get_parstr() const { return parstr;}
    bool        do_param_edits();
    // virtuals from dobj
    virtual errors::TYPE validate() { return errors::NO_ERROR; }
    bool        set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    char* name;
    char* parstr;
    bool mod_param_edit(synthmod::base*, const char* parname,
                                         const char* valstr);
    bool dobj_param_edit(dobj::base*, const char* parname, const char* valstr);
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
