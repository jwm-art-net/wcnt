#ifndef COPIER_H
#define COPIER_H

#include "dobj.h"

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

class synthmod;

class copier : public dobj
{
 public:
    copier();
    ~copier();
    // these two lookup dobj/synthmod names:
    bool set_from_name(const char*);
    bool set_to_name(const char*);
    // virtuals from dobj
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    errors::TYPE validate();

 private:
    char* from_name;
    char* to_name;
    synthmod* from_mod;
    synthmod* to_mod;
    dobj* from_dobj;
    dobj* to_dobj;
    void init_first();
    
};





#endif
