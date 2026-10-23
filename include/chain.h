#ifndef CHAIN_H
#define CHAIN_H

#include "dobjlist.h"
#include "synthmodlist.h"
#include "synthfilereader.h"

/*
    chain - a bit like grouping, but not
    ------------------------------------

    the existing grouping functionality via the group data object and autogroup
    directive, cannot nest groups within a group. there are two use cases where
    nested grouping might be desirable. one is the big case, for example a
    polyphonic system with multiple voices, where each voice consists of
    multiple layers (using a group for each layer) of sound.

    chain addresses the small use case where the user may set up a few
    modules for a specific task within a group. one example might be an lfo,
    consisting of 3 or more modules: clock, waveform, and lfo_controller.

    using chain will allow that lfo to be easily reproduced many times using
    the copier.

    chain has several behavioural differences to group:

        * the name changes of modules affected by chain serve only to prevent
          name clashes when the copier is used (vs grouping where the name
          change forms the mechanism for identifying group members).

        * a chain contains duplicates of the modules and their connections that
          it originally chained. these are independent of the original modules
          and are what is actually duplicated when the chain is duplicated by
          the copier.

        * when the copier is used on a chain - the chain data object is never
          copied, nor the modules first chained. it only every copies from it's
          own prviate internal lists of modules and connections which were
          created at the original point of chaining.

        * chain is the lowest level in the group heirarchy. the same chain
          can be copied into the same group multiple times or into multiple
          other groups multiple times.

        * however a chain is strong and can pull modules out of multiple groups
          and other chains simultaneously (when used as a data object, not when
          used as a directive).

        * but then, the copy_only parameter can be used to disable the pull of
          the transfer and assignment of ownership via name change to the chain.
          this is what neccessitates, when chain is used as a data object and
          not directive, tmpdbjlist to hold a list of the ui data objects
          pointing to the modules to be used, until do_actions is called.
          for otherwise, if the copy_only parameter was specified after the
          list of modules to chain, it would have been too late to use it.
*/

namespace synthmod { class base; }

class chain : public dobj::base
{
 public:
    chain();
    ~chain();

    // chain_module  - used when chain defined as data object
    synthmod::base* chain_module(synthmod::base*);
    // auto_chain_module - used during chain_create directive
    synthmod::base* autochain_module(synthmod::base*);

    // virtuals from dobj
    dobj::base* add_dobj(dobj::base*);
    errors::TYPE validate();
    dobj::base* duplicate_dobj(const char*);

    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

    bool do_actions();

    synthmod::list* get_modlist() const { return modules; }
    bool is_copy_only() { return copy_only == ON; }

 private:
    STATUS copy_only;
    synthmod::list*   modules;
    connectorlist*  connections;

    void register_ui();
    ui::dobjitem_list* get_ui_items();
    dobj::list* tmpdbjlist;
};

#endif
