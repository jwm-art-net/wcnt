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

namespace dobj
{
 class base
 {
 public:
    base(TYPE);
    virtual ~base();

    TYPE get_object_type() const { return object_type; }
    bool is_named_by_user();
    bool set_username(const char*);
    // some dobj are unamed by user, but contain another string which
    // is helpful when identifying errors, which is why get_username()
    // is now virtual.
    virtual const char*     get_username();
    bool                    is_valid() const   { return valid; }
    virtual errors::TYPE    validate() = 0;

    virtual bool        set_param(param::TYPE, const void*);
    virtual const void* get_param(param::TYPE) const;
    virtual const base* add_dobj(base*); // don't be fooled...
    virtual base*       duplicate_dobj(const char*);

    static const char* get_error_msg() { return err_msg; }

    bool operator()(TYPE & dt) const {
        return object_type == dt;
    }

    bool operator()(name & n) const {
        return n(username);
    }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

    void ui_register();

  protected:
    static char err_msg[STRBUFLEN];
    void invalidate(){ valid = false;}
    virtual void register_ui() = 0;
    void register_param(param::TYPE);
    void register_param(param::TYPE, const char* fixed_string);
    void register_dobj(TYPE parent, TYPE sprog);
    bool validate_param(param::TYPE, errors::TYPE);

  private:
    TYPE object_type;
    char* username;
    bool valid;

    #ifdef DATA_STATS
    STATS_VARS
    #endif

    static bool first_done[LAST_TYPE];
 };



#ifdef DEBUG
#define dobjerr(fmt, ... )                              \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(dobj::base::err_msg, STRBUFLEN, fmt, __VA_ARGS__);   \
}
#else
#define dobjerr(fmt, ... ) \
    cfmt(dobj::base::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif

}; // namespace dobj

#endif
