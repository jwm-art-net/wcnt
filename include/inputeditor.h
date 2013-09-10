#ifndef INPUTEDITOR_H
#define INPUTEDITOR_H

#include "inputedit.h"
#include "linkedlist.h"

class inputeditor : public dobj, public linked_list<inputedit>
{
 public:
    inputeditor();
    ~inputeditor();

    /*
    // creates the connectors from the inputedits.
    */
    bool create_connectors();

    /*
    // virtuals from dobj
    */
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }

    dobj const* add_dobj(dobj* dbj);

 private:
    void create_params();
    static bool done_params;
};

#endif
