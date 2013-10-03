#ifndef PARAMEDITOR_H
#define PARAMEDITOR_H

#include "paramedit.h"
#include "linkedlist.h"

class parameditor : public dobj, public linked_list<paramedit>
{
 public:
    parameditor();

    bool do_param_edits();

    /*
    // virtuals from dobj
    */
    virtual errors::TYPE validate(){
        return errors::NO_ERROR;
    }

    dobj const* add_dobj(dobj* dbj);

 private:
    void init_first();
};

#endif
