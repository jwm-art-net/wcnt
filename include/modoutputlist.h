#ifndef MODOUTPUTSLIST_H
#define MODOUTPUTSLIST_H

#include "linkedlist.h"
#include "modoutput.h"

class modoutputlist : public linked_list<modoutput>
{
 public:
    modoutputlist(){};
    modoutputlist(DESTRUCTION);

    modoutput* register_output(synthmod *, outputnames::OUT_TYPE);
    void delete_module_outputs(synthmod *);

    modoutputlist* list_of_category(iocat::IOCAT);

    #ifdef UNUSED
    bool is_registered(synthmod *);

    modoutputlist* list_of_category_orderpref(iocat::IOCAT out_cat,
                                    synthmodnames::SYNTHMOD_TYPE* sm_prefs,
                                    outputnames::OUT_TYPE* out_prefs,
                                    outputnames::OUT_TYPE* not_out_prefs);
    #endif
};


#endif
