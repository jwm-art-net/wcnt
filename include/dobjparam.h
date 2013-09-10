#ifndef DOBJPARAM_H
#define DOBJPARAM_H

#include "dobj.h"
#include "checkvalue.h"

#ifndef BARE_MODULES

// ties object parameters to an object
// the knot is a reference to an object type, and a refernce 
// to a dparam type.
// validate looks for the param type dobjparam holds, in the dobj,
// and then checks it's value is within the range expressed by
// ERR_TYPE.

class dobjparam
{
public:
    dobjparam(dobjnames::DOBJ_TYPE dt, dparamnames::DPAR_TYPE dpt);
    ~dobjparam(){};
    dobjnames::DOBJ_TYPE get_dobjtype() {
        return (this) ? dobjtype : dobjnames::DOBJ_FIRST;
    }
    dparamnames::DPAR_TYPE get_dpartype() {
        return (this) ? dpartype : dparamnames::DPAR_FIRST;
    }
    IOCAT get_dparam_category() {
        return (this) 
            ? dobj::get_dparnames()->get_category(dpartype)
            : CAT_FIRST;
    }
    bool validate(dobj*, stockerrs::ERR_TYPE);
private:
    dobjnames::DOBJ_TYPE dobjtype;
    dparamnames::DPAR_TYPE dpartype;
};

#endif
#endif
