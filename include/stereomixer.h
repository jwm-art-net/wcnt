#ifndef STEREOMIXER_H
#define STEREOMIXER_H

#include "stereochannel.h"
#include "linkedlist.h"

class stereomixer: public synthmod
{
public:
    stereomixer(char const*);
    ~stereomixer();
    stereo_channel* add_channel(stereo_channel* ch);
    stereo_channel* remove_channel(stereo_channel* ch);
    stereo_channel* goto_first() {
        return chan = (stereo_channel*)chlist->goto_first()->get_data();
    }
    stereo_channel* goto_last() {
        return chan = (stereo_channel*)chlist->goto_last()->get_data();
    }
    stereo_channel* goto_prev() {
        return chan = (stereo_channel*)chlist->goto_prev()->get_data();
    }
    stereo_channel* goto_next() {
        return chan = (stereo_channel*)chlist->goto_next()->get_data();
    }
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);
private:
    double out_left;
    double out_right;
    double master_level;
    double o_l;
    double o_r;
    linkedlist* chlist;
    ll_item* chitem;
    stereo_channel* chan;
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
