#ifndef MODOUTPUTSLIST_H
#define MODOUTPUTSLIST_H

#ifndef BARE_MODULES

#include "linkedlist.h"
#include "outputnames.h"
#include "modoutput.h"

class modoutputlist
{
public:
    modoutputlist();
    ~modoutputlist();
    modoutput* add_output(synthmod *, outputnames::OUT_TYPE);
    void delete_module_outputs(synthmod *);
    modoutput* goto_first() {
        return output = (modoutput*)
         (output_item = outlist->goto_first())->get_data();
    }
    modoutput* goto_last() {
        return output = (modoutput*)
         (output_item = outlist->goto_last())->get_data();
    }
    modoutput* goto_prev() {
        return output = (modoutput*)
         (output_item = outlist->goto_prev())->get_data();
    }
    modoutput* goto_next() {
        return output = (modoutput*)
         (output_item = outlist->goto_next())->get_data();
    }
    modoutputlist* list_of_category(IOCAT);
    modoutputlist* list_of_category_orderpref(
        IOCAT out_cat,
        synthmodnames::SYNTH_MOD_TYPE* sm_prefs,
        outputnames::OUT_TYPE* out_prefs,
        outputnames::OUT_TYPE* not_out_prefs);
    modoutputlist* get_outputlist_for_module(synthmod*);
    bool is_registered(synthmod *);
private:
    void delete_output(modoutput *);
    linkedlist *outlist;
    ll_item* output_item;
    modoutput *output;
};

#endif
#endif
