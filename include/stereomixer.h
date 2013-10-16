#ifndef STEREOMIXER_H
#define STEREOMIXER_H

#include "stereochannel.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class stereomixer: public synthmod::base,
                   public linked_list<synthmod::base>
{
 public:
    stereomixer(const char*);
    ~stereomixer();

    friend synthmod::base*
        duplicate_list_module<stereomixer, synthmod::base>
                                (stereomixer*, synthmod::base*,
                                const char* uname, synthmod::base::DUP_IO);
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod::base* duplicate_module(const char* uname, DUP_IO);
    dobj::base* add_dobj(dobj::base*);

 private:
    double out_left;
    double out_right;
    double master_level;
    double const** chans_left;
    double const** chans_right;
    void register_ui();
};

#endif
