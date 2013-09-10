#ifndef DOBJDOBJ_H
#define DOBJDOBJ_H

// used to tie dobj types to other dobj types without tying every
// instance of each.
// example for adsr (synthmod) :
//  * adds a moddobj with reference to adsr module type, and
//    reference to envelope dobj type
//  * envelope dobj adds a dobjdobj with a reference to
//    envelope dobj type, and coord dobj type
// when it comes to creating the adsr the program gets all moddobjs
// which reference adsr module type, and then gets all dobjdobj
// which reference envelope dobj type - the dobj type specified in
// the moddobj.  then it looks in the dobjdobj and reads the dobj
// type specified, ie coord dobj type.

#ifndef BARE_MODULES

#include "dobjnames.h"

class dobjdobj
{
public:
    dobjdobj(dobjnames::DOBJ_TYPE, dobjnames::DOBJ_TYPE);
    ~dobjdobj();
    dobjnames::DOBJ_TYPE get_dobj_type() {
        return (this) ? dobj_type : dobjnames::DOBJ_FIRST;
    }
    dobjnames::DOBJ_TYPE get_dobj_sprog() {
        return (this) ? sprog : dobjnames::DOBJ_FIRST;
    }
private:
    dobjnames::DOBJ_TYPE dobj_type;
    dobjnames::DOBJ_TYPE sprog;
};

#endif
#endif
