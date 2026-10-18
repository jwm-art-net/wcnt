#ifndef CHAIN_H
#define CHAIN_H

#include "dobj.h"
#include "synthmodlist.h"

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

    chain has behavioural differences to group:

        * the name changes of modules affected by chain serve only to prevent
          name clashes when the copier is used (vs grouping where the name
          change forms the mechanism for identifying group members).

        * a chain contains duplicates of the modules and their connections that
          it originally chained. these are independent of the original modules
          and are what is duplicated when the chain is duplicated by the copier.

        * when the copier is used on a chain - the chain data object is never
          copied, it only copies it's prviate internal lists of modules and
          connections

        * chain is the lowest level in the group heirarchy. the same chain
          can be copied into the same group multiple times or into multiple
          other groups multiple times.
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
    synthmod::base* auto_chain_module(synthmod::base*);

    // virtuals from dobj
    dobj::base* add_dobj(dobj::base*);
    errors::TYPE validate();
    dobj::base* duplicate_dobj(const char*);

 private:
    synthmod::list*   modules;
    connectorlist*  connections;

    bool chainify(synthmod::base*);

    void register_ui();
    ui::dobjitem_list* get_ui_items();
};


#endif
