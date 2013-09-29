#ifndef RIFFEDITOR_H
#define RIFFEDITOR_H

#include "dobj.h"

/*
    riff_editor
    -----------

    this is the way to add more notes to an already existing riff_data.
    Such that you can use special note names as editing commands already
    implimented in riff_data, the riff_editor does not actually need
    to do much. (but that could change depending on what I dream up for 
    it to do).

    all the riff_editor needs to do is be told the name of the riff_data
    to edit, and then, pass all the notes passed to it by the synthfile
    reader, straight into to the riff_data, and let riff_data do all the
    work such as storing and processing them etc.
*/

class riffdata;

class riff_editor : public dobj
{
 public:
    riff_editor();
    ~riff_editor();

    void set_riff_source(riffdata* rs)  { riff_source = rs;     }
    riffdata* get_riff_source()         { return riff_source;   }

    // virtuals from dobj
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    dobj const* add_dobj(dobj*);
    stockerrs::ERR_TYPE validate();

 private:
    riffdata* riff_source;
    void init_first();
    
};





#endif
