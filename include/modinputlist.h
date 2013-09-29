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

    modinput* register_input(synthmod*, input::TYPE);
    void delete_module_inputs(synthmod*);
};

/*
// unused, so it seems...

    modinputlist* get_list_of_category(iocat::TYPE);
    modinputlist* get_list_of_category_orderpref(
        iocat::TYPE in_cat,
        module::TYPE * sm_prefs,
        input::TYPE* in_prefs, 
        input::TYPE* not_in_prefs);
*/


#endif
