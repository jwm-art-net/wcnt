#ifndef STEREOMIXER_H
#define STEREOMIXER_H

#include "stereochannel.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class stereomixer: public synthmod, public linked_list<synthmod>
{
public:
    stereomixer(const char*);
    ~stereomixer();

    friend synthmod*
        duplicate_list_module<stereomixer, synthmod>
            (stereomixer* sm, synthmod* _data,
                const char* uname, synthmod::DUP_IO dupio);

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);
private:
    double out_left;
    double out_right;
    double master_level;
    double const** chans_left;
    double const** chans_right;
    
    void init_first();
};

#endif
