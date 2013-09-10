#ifndef STEREOMIXER_H
#define STEREOMIXER_H

#include "stereochannel.h"
#include "linkedlist.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "moddobjlist.h"
#include "dobjmod.h"
#include "dobjlist.h"

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
    const short* get_output_left(){ return &out_left;}
    const short* get_output_right(){ return &out_right;}
    void set_master_level(double ml) { master_level = ml;}
    double get_master_level(){ return master_level;}
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
    synthmod* duplicate_module(const char* uname, DUP_IO);
// stereo_channel is not a dobj, but a synthmod, so a dobj wrapper class
// - dobjmod, is passed which contains a pointer to the stereo_channel
    dobj* add_dobj(dobj*);
private:
    short out_left;
    short out_right;
    double master_level;
    double o_l;
    double o_r;
    linkedlist* chlist;
    ll_item* chitem;
    stereo_channel* chan;
    static int stereomixer_count;
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
