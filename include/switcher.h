#ifndef SWITCHER_H
#define SWITCHER_H

#include "wcntsignal.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class switcher: public synthmod::base, public linked_list<synthmod::base>
{
public:
    switcher(const char*);
    ~switcher();

    friend synthmod::base*
        duplicate_list_module<switcher, synthmod::base>
                                (switcher*, synthmod::base*,
                                const char* uname, synthmod::base::DUP_IO);
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod::base* duplicate_module(const char*, synthmod::base::DUP_IO);
    dobj::base* add_dobj(dobj::base*);
private:
    STATUS const* in_trig;
    double xfadetime;
    double out_output;
    samp_t xfade_samp;
    samp_t xfade_max_samps;
    double xfade_stpsz;
    double xfade_size;
    double const** sigs;
    long sig_ix;
    double const* sig;
    double const* prevsig;
    double zero;// for initial stuff
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
