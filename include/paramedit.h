#ifndef PARAMEDIT_H
#define PARAMEDIT_H

#include <stdio.h>
#include <sstream>

#include "dobjdobjlist.h"
#include "iocatconvert.h"

//  paramedit
// -----------

// two strings are stored by user interface, the first being
// taken as the module name, the second as a list of words
// and the words turn out to be alternating between a parameter
// name and it's value.

class paramedit : public dobj
{
public:
    paramedit();
    ~paramedit();
    void set_name(const char*);
    void set_parstr(const char*);
    const char* get_name() { return name; }
    const char* get_parstr() { return parstr;}
    bool do_param_edits(bool verbose);
    // virtuals from dobj
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    char* name;
    char* parstr;
    bool mod_param_edit(synthmod*, const char* parname, 
                        const char* valstr);
    bool dobj_param_edit(dobj*, const char* parname, const char* valstr);
    void create_params();
    static bool done_params;
};

#endif
