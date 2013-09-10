#ifndef DOBJCOPIER_H
#define DOBJCOPIER_H

#include "dobjparamlist.h"
#include "topdobjlist.h"

/*
    dobj_copier
    -----------
    copies a data object
*/


class riff_editor : public dobj
{
 public:
    riff_editor();
    ~riff_editor();

    void set_riff_source(riffdata* rs){ riff_source = rs; }
    riffdata* get_riff_source(){ return riff_source; }

    // virtuals from dobj
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE);
    dobj const* add_dobj(dobj*);
    stockerrs::ERR_TYPE validate();

 private:
    riffdata* riff_source;
    void create_params();
    static bool done_params;
};





#endif
