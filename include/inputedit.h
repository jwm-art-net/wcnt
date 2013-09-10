#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include <sstream>

#include "dobjdobjlist.h"
#include "dobjparamlist.h"
#include "synthmodulelist.h"

// inputedit
// ---------
//
// a dobj which allows module inputs to be reconnected
//
// two strings are stored by user interface, the first being
// taken as the module name, the second as a list of words
// and the words turn out to be alternating between an input name
// name, and the output module and it's output to connect to.

class inputedit : public dobj
{
public:
    inputedit();
    ~inputedit();
    void set_modname(const char*);
    void set_iostr(const char*);
    const char* get_modname() { return modname; }
    const char* get_iostr() { return iostr;}
    bool create_connectors(bool verbose);
    // virtuals from dobj
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    char* modname;
    char* iostr;
    void create_params();
    static bool done_params;
};

#endif
