#ifndef SYNTHMODULELIST_H
#define SYNTHMODULELIST_H

#include "synthmodule.h"
#include "synthmodnames.h"
#include "modinputslist.h"
#include "modoutputslist.h"

#include "modules.h"

class synthmodlist
{
public:
    synthmodlist();
    ~synthmodlist();
    synthmod* add_module(synthmod*);
    synthmod* create_module(synthmodnames::SYNTH_MOD_TYPE smt, char const*);
    bool delete_module(synthmod* const sm);
    synthmod* goto_first() {
        return smod = (synthmod*)
         (smod_item = smodlist->goto_first())->get_data();
    }
    synthmod* goto_last() {
        return smod = (synthmod*)
         (smod_item = smodlist->goto_last())->get_data();
    }
    synthmod* goto_prev() {
        return smod = (synthmod*)
         (smod_item = smodlist->goto_prev())->get_data();
    }
    synthmod* goto_next() {
        return smod = (synthmod*)
         (smod_item = smodlist->goto_next())->get_data();
    }
    synthmod* get_synthmod_by_name(char const*);
    synthmod* get_first_of_type(synthmodnames::SYNTH_MOD_TYPE);
    synthmod* get_next_of_type();
    // this should only be called before running the synth:
    // (it unlinks them and places them in constlist)
    void remove_constants(bool verbose);

private:
    linkedlist* smodlist;
    linkedlist* constlist;
    ll_item* smod_item;
    synthmod* smod;
    synthmodnames::SYNTH_MOD_TYPE search_modtype;
    ll_item* search_result;
};

#endif
