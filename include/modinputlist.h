#ifndef MODINPUTSLIST_H
#define MODINPUTSLIST_H

#include "linkedlist.h"
#include "inputnames.h"
#include "modinput.h"

/*
// NULL terminated array of modules to list first
// NULL terminated array of input types to list first
// NULL terminated array of input types to exclude

// sometime later I wonder what all that NULL terminated array
// business is all about.
*/

class synthmod;

class modinputlist
{
 public:
    modinputlist();
    ~modinputlist();
    modinput* add_input(synthmod*, inputnames::IN_TYPE);
    bool delete_module_inputs(synthmod*);
    modinput* goto_first(){ 
        return input = (modinput*)
         (input_item = inlist->goto_first())->get_data();
    }
    modinput* goto_last(){ 
        return input = (modinput*)
         (input_item = inlist->goto_last())->get_data();
    }
    modinput* goto_prev(){ 
        return input = (modinput*)
         (input_item = inlist->goto_prev())->get_data();
    }
    modinput* goto_next(){ 
        return input = (modinput*)
         (input_item = inlist->goto_next())->get_data();
    }
    modinputlist* get_list_of_category(iocat::IOCAT);
    modinputlist* get_list_of_category_orderpref(
        iocat::IOCAT in_cat,
        synthmodnames::SYNTH_MOD_TYPE * sm_prefs,
        inputnames::IN_TYPE* in_prefs, 
        inputnames::IN_TYPE* not_in_prefs);
    modinputlist* get_inputlist_for_module(synthmod*);

 private:
    bool delete_input(modinput*);
    linkedlist* inlist;
    ll_item* input_item;
    modinput* input;
};

#endif
