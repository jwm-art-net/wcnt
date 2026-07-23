#ifndef DOBJ_H
#define DOBJ_H

#include "stockerrs.h"
#include "types.h"
#include "paramnames.h"
#include "dobjnames.h"
#include "namefuncobj.h"
#include "textstuff.h"
#include "ui_dobjitem.h"

#ifdef DEBUG
#include <cstdio>
#endif


namespace dobj
{
 class base
 {
 public:
    enum DO_FLAGS
    {
      DO_VALID    =           0x0001,
      // the following apply only to standalone data objects (ie copier
      // riff, wavfilein etc), these flags are irrelevant to data objects
      // defined within something else (ie shape, editlist, signals etc):
      DO_EDITABLE =           0x0002,
      DO_ALLOW_IN_AUTOGROUP = 0x0004,
      DO_ALLOW_NAMELESS =     0x0008
    };
    base(TYPE);
    virtual ~base();

    TYPE get_object_type() const { return object_type; }
    bool is_named_by_user();
    bool set_username(const char*);
    // some dobj are unamed by user, but contain another string which
    // is helpful when identifying errors, which is why get_username()
    // is now virtual.
    virtual const char*   get_username();
    bool is_valid() const { return flags & DO_VALID; }
    bool is_allow_in_autogroup() const { return flags & DO_ALLOW_IN_AUTOGROUP; }
    bool is_allow_nameless() const { return flags & DO_ALLOW_NAMELESS; }

    virtual errors::TYPE  validate() = 0;

    virtual bool        set_param(param::TYPE, const void*);
    virtual const void* get_param(param::TYPE) const;
    virtual const base* add_dobj(base*); // don't be fooled...
    virtual base*       duplicate_dobj(const char*);
    bool is_editable() { return flags & DO_EDITABLE; }

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
    virtual ui::dobjitem_list* get_ui_items() = 0;

  protected:
    static char err_msg[STRBUFLEN];
    void invalidate(){ flags &=~ DO_VALID;}
    virtual void register_ui() = 0;
    ui::dobjparam*   register_param(param::TYPE);
    ui::dobjparam*   register_param(param::TYPE, const char* fixed_string);
    ui::dobjdobj*    register_dobj(TYPE parent, TYPE sprog);
    ui::dobjcomment* register_comment(const char* literal);
    bool    validate_param(param::TYPE, errors::TYPE);
    void set_flags(int f) { flags |= f; }

  private:
    TYPE object_type;
    char* username;
    int flags;

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
