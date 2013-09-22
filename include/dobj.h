#ifndef DOBJ_H
#define DOBJ_H

#include "stockerrs.h"
#include "types.h"
#include "paramnames.h"
#include "dobjnames.h"
#include "namefuncobj.h"
#include "textstuff.h"

#ifdef DEBUG
#include <cstdio>
#endif


class dobj
{
 public:
    dobj(dobjnames::DOBJ_TYPE objtype);
    virtual ~dobj();

    dobjnames::DOBJ_TYPE get_object_type() const { return object_type; }
    bool is_named_by_user();
    bool set_username(const char*);
    // some dobj are unamed by user, but contain another string which
    // is helpful when identifying errors, which is why get_username()
    // is now virtual.
    virtual char const* get_username();
    bool is_valid() const   { return valid; }
    virtual stockerrs::ERR_TYPE validate() = 0;
    virtual bool set_param(paramnames::PAR_TYPE, void*);
    virtual void const* get_param(paramnames::PAR_TYPE) const;
    virtual dobj const* add_dobj(dobj*); // don't be fooled...
    virtual dobj* duplicate_dobj(const char*);

    static const char* get_error_msg() { return err_msg; }

    bool operator()(dobjnames::DOBJ_TYPE & dt) const {
        return object_type == dt;
    }

    bool operator()(name & n) const {
        return n(username);
    }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

protected:
    static char err_msg[STRBUFLEN];
    virtual void init_first() = 0;
    void invalidate(){ valid = false;}
    void register_param(paramnames::PAR_TYPE);
    void register_param(paramnames::PAR_TYPE, const char* fixed_string);
    void register_dobjdobj(dobjnames::DOBJ_TYPE parent,
                                                dobjnames::DOBJ_TYPE sprog);
    bool done_first() const;

 private:
    dobjnames::DOBJ_TYPE object_type;
    char* username;
    bool valid;

    #ifdef DATA_STATS
    STATS_VARS
    #endif

    static bool first_done[dobjnames::DOBJ_LAST];
};


#ifdef DEBUG
#define dobjerr(fmt, ... )                              \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(dobj::err_msg, STRBUFLEN, fmt, __VA_ARGS__);   \
}
#else
#define dobjerr(fmt, ... ) \
    cfmt(dobj::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif


#endif
