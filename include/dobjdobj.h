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

#include "dobjnames.h"

class dobjdobj
{
public:
    dobjdobj(dataobj::TYPE, dataobj::TYPE);
    dobjdobj(dobjdobj &);
    ~dobjdobj();
    dataobj::TYPE get_dobj_type() const {
        return (this) ? dobj_type : dataobj::ERR_TYPE;
    }
    dataobj::TYPE get_dobj_sprog() const {
        return (this) ? sprog : dataobj::ERR_TYPE;
    }

private:
    dataobj::TYPE dobj_type;
    dataobj::TYPE sprog;
};

#endif
