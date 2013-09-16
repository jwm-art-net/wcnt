#ifndef DOBJ_H
#define DOBJ_H

#include <string>

#include "stockerrs.h"
#include "paramnames.h"
#include "dobjnames.h"
#include "namefuncobj.h"

class dobj
{
public:
    dobj(dobjnames::DOBJ_TYPE objtype);
    virtual ~dobj();
    dobjnames::DOBJ_TYPE get_object_type() { return object_type; }
    bool is_named_by_user();
    bool set_username(const char*);
    // some dobj are unamed by user, but contain another string which
    // is helpful when identifying errors, which is why get_username()
    // is now virtual.
    virtual char const* get_username();
    bool is_valid() { return valid; }
    virtual stockerrs::ERR_TYPE validate() = 0;
    virtual bool set_param(paramnames::PAR_TYPE, void*);
    virtual void const* get_param(paramnames::PAR_TYPE) const;
    virtual dobj const* add_dobj(dobj*); // don't be fooled...
    virtual dobj* duplicate_dobj(const char*);

    static void clear_error_msg(){ *err_msg = "";}
    static void register_error_msg(std::string* e)  { err_msg = e;    }
    static std::string const* get_error_msg()       { return err_msg; }

    bool operator()(dobjnames::DOBJ_TYPE & dt) {
        return object_type == dt;
    }

    bool operator()(name & n) const {
        return n(username);
    }

#ifdef DOBJ_STATS
STATS_FUNCS
#endif

protected:
    static std::string* err_msg;
    virtual void create_params() = 0;
    void invalidate(){ valid = false;}
    bool done_params();

private:
    dobjnames::DOBJ_TYPE object_type;
    char* username;
    bool valid;

#ifdef DOBJ_STATS
STATS_VARS
#endif

    static bool params_done[dobjnames::DOBJ_LAST];
};

#endif
