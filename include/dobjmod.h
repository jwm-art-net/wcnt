#ifndef DOBJMOD_H
#define DOBJMOD_H

#include "dobj.h"

#include "synthmod.h"

/*
dobjmod.h

for passing synth modules as dobj objects

the differences between dobj and synthmod are :-

-------------------------------------
generic		synthmod	dobj
-------------------------------------
inputs		yes			no		
outputs		yes			no
params		yes			yes
run			yes			no
lists		no			yes		
-------------------------------------
note:  several synthmods use lists of 
data, but they are strictly specialised
lists providing only functionality for
the data type that particular synthmod 
uses.
-------------------------------------

dobj handles classes of data which provide functionality to the 
synthmods without actually being a synthmod.  dobj provides a mechanisim
for inserting an unknown number of objects into a list. it provides a
generic way of doing this so that specialist code within the user
interface can be erradicated.

    example 1: adsr is a synthmod, but it uses a dobj reference to an
     envelope which is a virtual dobj, ie no actual class is derived
     from dobj called envelope.  the envelope reference, refers to a
     dobj object named section which contains the parameters for the
     adsr. there can be any number of sections in an adsr.

    example 2: a dobj called riff contains a list of dobjs which are
     notes in the riff.  the riffs are inserted into the sequencer as
     parameters of another dobj called riff_node, There can be any 
     number of notes in a riff and any number of riff_nodes in a
     sequencer.

dobjmod
-------
when a synthmod contains a list of other synthmods, for example, the
stereo_mixer module contains a list of stereo_channel modules, the parent
synthmod adds a moddobj to link the synthmod type with a dobj type and
then adds dobjdobj which links the dobj type in the moddobj, to the (ha!)
DOBJ_SYNTHMOD dobj type.  Unlike other dobj types, the dobjmod is only a
wrapper for the object (synthmod) it contains, the container synthmod does
not work with dobjmods, but the specific module type.  the add_dobj method
extracts the synthmod from the dobjmod.  Because the synthmod does not
actually use the dobjmod for any other purpose, and the add_dobj method
returns the dobj it was passed when successful, the dobjmod cannot be
deleted there and then. 

Also the synthfile reader code will not delete it because it thinks that
it's the synthmod's job.  This leaves the only place it can be deleted as
the destructor of the dobjlist, but the dobjlist is for dobjs which are
defined outside of a synthmodule (ie riff/wavfilein).  But luckily enough,
these dobjs all require a user name, and it is not possible for them to
have no name, and it is not possible for the user to reference a dobj
without a name, so the nameless dobjmods in the list will be invisible to
the user and will be deleted when wcnt exits.

****************************	
 ***the add_dobj method ***
  ***of a module which ***
  ***uses dobjmod must ***
    ***add the dobjmod**
    ***to the dobjlist**
     ***************** ---------- hope that's clear for you.
*/


class dobjmod : public dobj::base
{
public:
    dobjmod();
    ~dobjmod(){};
    void set_synthmod(synthmod::base* sm){ synth_mod = sm;}
    synthmod::base* get_synthmod(){ return synth_mod;}
    virtual errors::TYPE validate(){ 
        return errors::NO_ERROR; 
    }
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE pt) const;

private:
    synthmod::base* synth_mod;
    void register_ui();
};

#endif
