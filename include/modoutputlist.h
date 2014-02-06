#ifndef MODOUTPUTSLIST_H
#define MODOUTPUTSLIST_H

#include "linkedlist.h"
#include "modoutput.h"

class modoutputlist : public linked_list<modoutput>
{
 public:
    modoutputlist() {}
    modoutputlist(DESTRUCTION);

    modoutput* register_output(synthmod::base*, int output_type);
    void delete_module_outputs(synthmod::base*);

    modoutputlist* list_of_category(iocat::TYPE);

    #ifdef UNUSED
    bool is_registered(synthmod *);

    modoutputlist* list_of_category_orderpref(iocat::TYPE out_cat,
                                             synthmod::TYPE* sm_prefs,
                                             int* out_prefs,
                                             int* not_out_prefs);
    #endif
};


#endif
