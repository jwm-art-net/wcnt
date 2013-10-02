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

class synthmod;

class paramedit : public dobj
{
public:
    paramedit();
    ~paramedit();
    bool set_name(const char*);
    void set_parstr(const char*);
    const char* get_name() const { return name; }
    const char* get_parstr() const { return parstr;}
    bool do_param_edits();
    // virtuals from dobj
    virtual errors::TYPE validate(){
        return errors::NO_ERROR;
    }
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    char* name;
    char* parstr;
    bool mod_param_edit(synthmod*, const char* parname, 
                        const char* valstr);
    bool dobj_param_edit(dobj*, const char* parname, const char* valstr);
    void init_first();
    
};

#endif
