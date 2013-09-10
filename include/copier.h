#ifndef COPIER_H
#define COPIER_H

#include "dobj.h"
#include "synthmodule.h"


/*
    copier
    -----------
    copies a data object or synthmodule

    the set_from_name method checks that a data object or module
    exists with the specified name.
    the set_to_name methods check that no data object or module
    exists with the specified name.

    the validate method actually does the copy and adds it to the
    appropriate storage list, be it the dobj list or mod list.
*/


class copier : public dobj
{
 public:
    copier();
    ~copier();

    bool set_from_name(const char*);
    bool set_to_name(const char*);
    const char* get_from_name(){ return from_name;}
    const char* get_to_name(){ return to_name;}

    // virtuals from dobj
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE);
    stockerrs::ERR_TYPE validate();

 private:
    char* from_name;
    char* to_name;
    synthmod* from_mod;
    synthmod* to_mod;
    dobj* from_dobj;
    dobj* to_dobj;
    void create_params();
    static bool done_params;
};





#endif
