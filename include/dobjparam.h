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
    dobjparam(dataobj::TYPE dt, param::TYPE pt);
    ~dobjparam(){};
    dataobj::TYPE get_dobjtype() const {
        return (this) ? dobjtype : dataobj::ERR_TYPE;
    }
    param::TYPE get_partype() const {
        return (this) ? partype : param::ERR_TYPE;
    }
    iocat::TYPE get_dparam_category() const {
        return (this) 
            ? param::names::category(partype)
            : iocat::ERR_TYPE;
    }
    bool validate(dobj*, errors::TYPE);

    bool operator()(dataobj::TYPE & dt) const {
        return dobjtype == dt;
    }

    bool operator()(param::TYPE & pt) const {
        return partype == pt;
    }

private:
    dataobj::TYPE dobjtype;
    param::TYPE partype;
};

#endif
