#ifndef DOBJPARAM_H
#define DOBJPARAM_H

#include "dobjnames.h"
#include "paramnames.h"
#include "stockerrs.h"
#include "jwm_globals.h"

class dobj;

// ties object parameters to an object
// the knot is a reference to an object type, and a refernce 
// to a dparam type.
// validate looks for the param type dobjparam holds, in the dobj,
// and then checks it's value is within the range expressed by
// ERR_TYPE.

class dobjparam
{
public:
    dobjparam(dobjnames::DOBJ_TYPE dt, paramnames::PAR_TYPE pt);
    ~dobjparam(){};
    dobjnames::DOBJ_TYPE get_dobjtype() {
        return (this) ? dobjtype : dobjnames::DOBJ_FIRST;
    }
    paramnames::PAR_TYPE get_partype() {
        return (this) ? partype : paramnames::FIRST;
    }
    iocat::IOCAT get_dparam_category() {
        return (this) 
            ? jwm.get_paramnames()->get_category(partype)
            : iocat::FIRST;
    }
    bool validate(dobj*, stockerrs::ERR_TYPE);

    bool operator()(dobjnames::DOBJ_TYPE & dt) const {
        return dobjtype == dt;
    }

    bool operator()(paramnames::PAR_TYPE & pt) const {
        return partype == pt;
    }

private:
    dobjnames::DOBJ_TYPE dobjtype;
    paramnames::PAR_TYPE partype;
};

#endif
