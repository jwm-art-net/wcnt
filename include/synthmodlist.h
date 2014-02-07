#ifndef SYNTHMODULELIST_H
#define SYNTHMODULELIST_H

#include "synthmodnames.h"
#include "linkedlist.h"
#include "listwork.h"
#include "function_objects.h"
#include "boolfuncobj.h"

class nonezero;
#include "synthmod.h"

namespace synthmod
{
 class list : public linked_list<base>
 {
  public:
    list(DESTRUCTION);
    ~list();

    static synthmod::base* create_module(int synthmod_type, const char*);

    synthmod::base* add_module(synthmod::base* sm) {
        return add_at_tail(sm)->get_data();
    }

    bool delete_module(synthmod::base*);

    synthmod::base* get_synthmod_by_name(const char* const n) {
        return find_in_data(sneak_first(), fnobj::name(n))->get_data();
    }

    synthmod::base* get_first_of_type(int synthmod_type) {
        result = find_in_data(sneak_first(), search = synthmod_type);
        return result->get_data();
    }

    synthmod::base* get_next_of_type() {
        result = find_in_data(result->get_next(), search);
        return result->get_data();
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
    // returns 0 terminated synthmod::base* array:
    */
    synthmod::base** get_run_list() {
        return move_to_array(this);
    }

    /* remove_modules_of_group
    //  removes a specific group's modules from this, into a new
    //  synthmodlist.
    */
    linkedlist* remove_modules_of_group(const char* const n) {
        return move_to_new_list_of_by(this, fnobj::groupname(n));
    }

    /* validate_modules & init_modules
    //  removed from jwmsynth to here:
    //  * validate_modules should be called before jwmsynth::connect_synth
    //  * init_modules should be called after jwmsynth::connect_synth
    */

  private:
    linkedlist*         emptyrunlist;
    int                 search;
    llitem*             result;
    static nonezero*    off;
 };
}; // namespace synthmod

#endif
