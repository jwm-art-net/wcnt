#ifndef SYNTHMODLIST_H
#define SYNTHMODLIST_H

#include "synthmodnames.h"
#include "linkedlist.h"
#include "listwork.h"
#include "namefuncobj.h"
#include "boolfuncobj.h"
#include "synthmod.h"

class nonezero;
class group;

namespace synthmod
{
 class list : public linked_list<base>
 {
  public:
    list(DESTRUCTION);
    ~list();

    static synthmod::base* create_module(synthmod::TYPE, const char*);

    synthmod::base* add_module(synthmod::base* sm) {
        return add_at_tail(sm)->get_data();
    }

    bool delete_module(synthmod::base*);


    bool    set_autogroup(group*);
    group*  get_autogroup() { return autogroup; }
    void    clear_autogroup() { autogroup = 0; }

    // get_synthmod_by_name
    // matches a module with identical name
    synthmod::base* get_synthmod_by_name(const char* const modname);

    // if autogrouping, get synthmod by autogroup groupp modname,
    // if not found or not autogrouping, returns 0.
    synthmod::base* autogroup_only_get_synthmod_by_name(const char* const modname,
                                                            char** grpmodname);
    // if autogrouping get synthmod by autogroup group modname,
    // if not autogrouping get synthmod by name. return 0 if not found.
    synthmod::base* autogroup_or_not_get_synthmod_by_name(const char* const modname,
                                                          char** grpmodname);

    // if autogrouping get synthmod by autogroup group modname,
    // if not found, regardless of autogrouping, get synthmod by name
    synthmod::base* autogroup_or_any_get_synthmod_by_name(const char* const modname,
                                                             char** grpmodname);

    synthmod::base* get_first_of_type(synthmod::TYPE smt) {
        result = find_in_data(sneak_first(), search = smt);
        return result ? result->get_data() : 0;
    }

    synthmod::base* get_next_of_type() {
        result = find_in_data(result->get_next(), search);
        return result->get_data();
    }

    // dirty shortcut to determine off status of an input
    bool input_ptr_is_off(const void* input);

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
        return move_to_new_list_of_by(this, groupname(n));
    }

    /* list_of_modules_of_group
     *    //  adds a specific group's modules from this, into a new
     *    //  synthmodlist.
     */
    linkedlist* list_of_modules_of_group(const char* const n) {
        return new_list_of_by(this, groupname(n));
    }

    /* validate_modules & init_modules
    //  removed from jwmsynth to here:
    //  * validate_modules should be called before jwmsynth::connect_synth
    //  * init_modules should be called after jwmsynth::connect_synth
    */

  private:
    linkedlist*         emptyrunlist;
    synthmod::TYPE      search;
    llitem*             result;
    group*              autogroup;
    static nonezero*    off;
 };
}; // namespace synthmod

#endif
