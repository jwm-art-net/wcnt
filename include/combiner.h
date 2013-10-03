#ifndef COMBINER_H
#define COMBINER_H

#include "synthmod.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class wcnt_signal;

class combiner: public synthmod, public linked_list<synthmod>
{
public:
    combiner(const char*);
    ~combiner();

    friend synthmod*
        duplicate_list_module<combiner, synthmod>
            (combiner* sm, synthmod* _data,
                const char* uname, synthmod::DUP_IO dupio);

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    // wcnt_signal is not a dobj, but a synthmod, so a dobj wrapper class
    // - dobjmod, is passed which contains a pointer to the wcnt_signal
    dobj* add_dobj(dobj*);
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);

private:
    double out_output;
    STATUS meantotal;
    double total;
    double const** sigs;
    int sigcount;
    void init_first();
};

#endif
