#ifndef COMBINER_H
#define COMBINER_H

#include "synthmod.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class wcnt_signal;

class combiner: public synthmod, public linked_list<wcnt_signal>
{
public:
    combiner(char const*);
    ~combiner();

    friend synthmod*
        duplicate_list_module<combiner, wcnt_signal>
            (combiner* sm, wcnt_signal* _data,
                const char* uname, synthmod::DUP_IO dupio);

    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    // wcnt_signal is not a dobj, but a synthmod, so a dobj wrapper class
    // - dobjmod, is passed which contains a pointer to the wcnt_signal
    dobj* add_dobj(dobj*);
    void const* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);

private:
    double out_output;
    double meantotal;
    double total;
    wcnt_signal** wcntsigs;
    wcnt_signal* wcntsig;
    int sigcount;
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
