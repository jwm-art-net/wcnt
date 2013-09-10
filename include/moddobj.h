#ifndef MODDOBJ_H
#define MODDOBJ_H

// used to tie dobj types to synthmod types without
// tying every instance of each.

// moddobj is like modparam, once a module type has created a set
// of it's params, any other modules created, of that type will
// not create more moddobj.

// dobj is like synthmodule, but is part of every non synthmodule
// object which is defined by the user.  some dobj will be stored
// in a list, ie those which are not defined inside a specific module.
// the dobj in the list can be used by more than one module of the type
// there are intended for.

// because some dobj may be used with more than one of a module, and
// because the user is required to type an object header, and in some
// cases an object username.

// err right then.

// notice:
//  moddobj is used when a synth module contains the list object
//		ie is not a dobj inhereted class

#ifndef BARE_MODULES

#include "synthmodnames.h"
#include "dobjnames.h"

class moddobj
{
public:
    moddobj(synthmodnames::SYNTH_MOD_TYPE, dobjnames::DOBJ_TYPE);
    ~moddobj();
    synthmodnames::SYNTH_MOD_TYPE get_moduletype() {
        return (this) ? synthmodule_type : synthmodnames::MOD_FIRST;
    }
    dobjnames::DOBJ_TYPE get_dobj_type() {
        return (this) ? dobj_type : dobjnames::DOBJ_FIRST;
    }
private:
    synthmodnames::SYNTH_MOD_TYPE synthmodule_type;
    dobjnames::DOBJ_TYPE dobj_type;
};

#endif
#endif
