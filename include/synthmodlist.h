#ifndef SYNTHMODULELIST_H
#define SYNTHMODULELIST_H

#include "synthmodnames.h"
#include "linkedlist.h"
#include "listwork.h"
#include "namefuncobj.h"
#include "boolfuncobj.h"

class synthmod;
class nonezero;
#include "synthmod.h"

class synthmodlist : public linked_list<synthmod>
{
public:
    synthmodlist(DESTRUCTION);
    ~synthmodlist();

    synthmod* add_module(synthmod* sm) {
        return add_at_tail(sm)->get_data();
    }

    synthmod* create_module(module::TYPE smt,const char* );

    bool delete_module(synthmod* sm);

    synthmod* get_synthmod_by_name(const char* const n) {
        return find_in_data(sneak_first(), name(n))->get_data();
    }

    synthmod* get_first_of_type(module::TYPE smt) {
        return (search_result =
            find_in_data(sneak_first(), search_type = smt)
                )->get_data();
    }

    synthmod* get_next_of_type() {
        return (search_result =
            find_in_data(search_result->get_next(), search_type)
                )->get_data();
    }

    /*
    // duplicate_group returns 0 on failure or list of
    // duplicated modules on sucess.
    // it is assumed there are modules grouped as 'from'.
    */
    linkedlist* duplicate_group(const char* from, const char* to);

    /* remove_empty_run_modules
    //  this should only be called before running the synth:
    //  (it unlinks them and places them in emptyrunlist)
    */
    void remove_empty_run_modules();

    /*
    // returns 0 terminated synthmod* array:
    */
    synthmod** get_run_list() {
        return move_to_array(this);
    }

    /* remove_modules_of_group
    //  removes a specific group's modules from this, into a new
    //  synthmodlist.
    */
    linkedlist* remove_modules_of_group(const char* const n) {
        return move_to_new_list_of_by(this, groupname(n));
    }

    /* validate_modules & init_modules
    //  removed from jwmsynth to here:
    //  * validate_modules should be called before jwmsynth::connect_synth
    //  * init_modules should be called after jwmsynth::connect_synth
    */

private:
    linkedlist* emptyrunlist;
    module::TYPE search_type;
    llitem* search_result;
    static nonezero* off;
};

#endif
