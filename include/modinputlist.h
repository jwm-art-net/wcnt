#ifndef MODINPUTSLIST_H
#define MODINPUTSLIST_H

#include "linkedlist.h"
#include "listwork.h"
#include "inputnames.h"
#include "modinput.h"

class synthmod;

class modinputlist : public linked_list<modinput>
{
 public:
    modinputlist(){};
    modinputlist(DESTRUCTION);

    modinput* register_input(synthmod*, inputnames::IN_TYPE);
    void delete_module_inputs(synthmod*);
};

/*
// unused, so it seems...

    modinputlist* get_list_of_category(iocat::IOCAT);
    modinputlist* get_list_of_category_orderpref(
        iocat::IOCAT in_cat,
        synthmodnames::SYNTHMOD_TYPE * sm_prefs,
        inputnames::IN_TYPE* in_prefs, 
        inputnames::IN_TYPE* not_in_prefs);
*/


#endif
