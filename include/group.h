#ifndef GROUP_H
#define GROUP_H

#include "dobj.h"
#include "synthmodule.h"


/*
    group
    -----
    groups a bunch of modules together by renaming them.
    contains a list of modules to be added to the group.

    The name of the group is the name of the group data object defined.
    all the modules added to the group get renamed with the
    group name plus a '.' and then the modules existing names.

    all the connecters for the modules are then recreated to reflect
    the new names of the modules, and to maintain their relationships.

    the name of a module is the only thing connecting it to a group,
    that is, it's name is changed, nothing else, so when connecting one
    of it's outputs to an input, you must use the group name and module
    name.

    how I imagine an example.
    
    group
    group1
        name g1
        modulelist
            mod name adsr1
            mod name sine1
        modulelist
    group1

    amp1
        in_signal   g1.sine1
        in_amp_eg   g1.adsr1
        ....
    emp1

----------------
    intentions
    ----------
    the plan is to then have a group_clone data object which will clone
    all the modules of a group into a new group.

    synthfilereader code will have to be modified to show an error
    message and prevent modules from being created if they have a
    '.' in their name.

    when a dobj is added to a group, it should not be part of another
    group already.

    note: do not necessarily need a group_clone data object. can use
    the virtual duplicate method. could be tricky though. do not want
    to end up with modules in multiple groups (ie g2.g1.adsr1 - bad).
    if we are to use the group::add_module method from duplicate, then
    problems arise because the module is already in a group. I'll
    come to that when I get there...

    ...well I've not got anywhere near there, but have created a
    private constructor which has a single arguement - a private
    enum. the duplicate method will use the private constructor
    so that when it adds modules to the duplicate group, they
    will not be renamed, as they had to be renamed to be
    duplicated in the first instance.

    the cancel_duplicate_status is called once the duplication
    process is complete.

----------------
*/


class group : public dobj
{
 public:
    group();
    ~group();

    synthmod* add_module(synthmod* s);
    synthmod* goto_first() {
        return mod = (synthmod*)
         (mod_item = modlist->goto_first())->get_data();
    }
    synthmod* goto_last() {
        return mod = (synthmod*)
         (mod_item = modlist->goto_last())->get_data();
    }
    synthmod* goto_prev() {
        return mod = (synthmod*)
         (mod_item = modlist->goto_prev())->get_data();
    }
    synthmod* goto_next() {
        return mod = (synthmod*)
         (mod_item = modlist->goto_next())->get_data();
    }
    void cancel_duplicate_status(){is_duplicate = false;}
    // virtuals from dobj
    dobj* add_dobj(dobj*);
    stockerrs::ERR_TYPE validate();
    dobj* duplicate_dobj(const char*);
 private:
    enum CLONE {DUPLICATE = 1};
    group(CLONE);
    bool groupify(synthmod*);
    bool is_duplicate;
    linkedlist* modlist;
    ll_item* mod_item;
    synthmod* mod;
    void create_params();
    static bool done_params;
};





#endif
