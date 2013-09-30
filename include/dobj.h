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
    dobj(dataobj::TYPE);
    virtual ~dobj();

    dataobj::TYPE get_object_type() const { return object_type; }
    bool is_named_by_user();
    bool set_username(const char*);
    // some dobj are unamed by user, but contain another string which
    // is helpful when identifying errors, which is why get_username()
    // is now virtual.
    virtual const char* get_username();
    bool is_valid() const   { return valid; }
    virtual errors::TYPE validate() = 0;
    virtual bool set_param(param::TYPE, const void*);
    virtual const void* get_param(param::TYPE) const;
    virtual dobj const* add_dobj(dobj*); // don't be fooled...
    virtual dobj* duplicate_dobj(const char*);

    static const char* get_error_msg() { return err_msg; }

    bool operator()(dataobj::TYPE & dt) const {
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
    void register_param(param::TYPE);
    void register_param(param::TYPE, const char* fixed_string);
    void register_dobjdobj(dataobj::TYPE parent, dataobj::TYPE sprog);
    bool done_first() const;
    bool validate_param(param::TYPE, errors::TYPE);

 private:
    dataobj::TYPE object_type;
    char* username;
    bool valid;

    #ifdef DATA_STATS
    STATS_VARS
    #endif

    static bool first_done[dataobj::LAST_TYPE];
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
