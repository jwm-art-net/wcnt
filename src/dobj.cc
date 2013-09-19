#include "../include/dobj.h"
#include "../include/jwm_globals.h"
#include "../include/dobjlist.h"
#include "../include/dobjparamlist.h"
#include "../include/fxsparamlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/topdobjlist.h"

dobj::dobj(dobjnames::DOBJ_TYPE dt) :
 object_type(dt), username(0), valid(true)
{
    *err_msg = ""; // hmmm

#ifdef DOBJ_STATS
STATS_INC
#endif

}

dobj::~dobj()
{
    if (username)
        delete [] username;

#ifdef DOBJ_STATS
STATS_DEC
#endif

}

bool dobj::is_named_by_user()
{
    if (jwm.get_dobjnames()->check_type(object_type)
        == dobjnames::DOBJ_FIRST)
    {
        *err_msg = "\nError in object ";
        if (username) *err_msg += username;
        *err_msg += " type ";
        *err_msg += jwm.get_dobjnames()->get_name(object_type);
        return (valid = false);
    }
    // check_type(object_type) means need only check for 1 condition...
    if (object_type > dobjnames::DOBJ_DEFS)
        return true;
    return false; // should not be named by user
}

bool dobj::set_username(const char* un)
{
    if (!is_named_by_user()) {// should not be given username
        *err_msg = "\ntried to set username of object type ";
        *err_msg += jwm.get_dobjnames()->get_name(object_type);
        *err_msg += " - does not required naming";
        return (valid = false);
    }
    if (un == 0) {
        *err_msg = "\nNULL username specified for object type ";
        *err_msg += jwm.get_dobjnames()->get_name(object_type);
        return (valid = false);
    }
    if (strlen(un) == 0) {
        *err_msg = "\nNULL length username specified for object type ";
        *err_msg += jwm.get_dobjnames()->get_name(object_type);
        return (valid = false);
    }
    if (username) delete [] username;
    username = new char[strlen(un) + 1];
    strcpy(username, un);
    return true;
}

char const* dobj::get_username()
{
    if (jwm.get_dobjnames()->check_type(object_type)
        == dobjnames::DOBJ_FIRST)
        return 0;
    return username;
}

bool dobj::set_param(paramnames::PAR_TYPE, void*)
{
    *err_msg =
        "\n***programmer error***\nattempt made to set parameter data"
        "\nof data object lacking capability to set them.";
    return 0;
}

void const* dobj::get_param(paramnames::PAR_TYPE) const
{
    *err_msg =
        "\n***programmer error***\nattempt made to get parameter data"
        "\nfrom data object lacking capability to provide them.";
    return 0;
}

dobj const* dobj::add_dobj(dobj*)
{
    *err_msg =
        "\n***programmer error***\nattempt made to add data object to"
        "\ndata object lacking ability to accept them";
    return 0;
}

dobj* dobj::duplicate_dobj(const char* uname)
{
    *err_msg = "from:\ndata object ";
    if (username) *err_msg += username;
    *err_msg += " of data type ";
    *err_msg += jwm.get_dobjnames()->get_name(object_type);
    *err_msg += " does not allow copies to be made of it.";
    return 0;
}

std::string*  dobj::err_msg = 0;

bool dobj::done_first()
{
    bool r = first_done[object_type];
    first_done[object_type] = true;
    return r;
}

void dobj::register_param(paramnames::PAR_TYPE pt)
{
    if (!valid)
        return;
    if (!jwm.get_dparlist()->add_dobjparam(object_type, pt))
    {
        *err_msg = "Failed to register param ";
        *err_msg += jwm.get_paramnames()->get_name(pt);
        *err_msg += " with data object type ";
        *err_msg += jwm.get_dobjnames()->get_name(object_type);
        *err_msg += ".";
        valid = false;
    }
}

void dobj::register_param(paramnames::PAR_TYPE pt, const char* fixstr)
{
    if (!valid)
        return;
    dobjparam* dp = jwm.get_dparlist()->add_dobjparam(object_type, pt);
    fixstrparam* fsp = 0;
    if (dp)
        fsp = jwm.get_fxsparamlist()->add_param(fixstr, pt);
    if (!dp || !fsp)
    {
        *err_msg = "Failed to register fixed string param ";
        *err_msg += jwm.get_paramnames()->get_name(pt);
        *err_msg += " (";
        *err_msg += fixstr;
        *err_msg += ") with data object type ";
        *err_msg += jwm.get_dobjnames()->get_name(object_type);
        *err_msg += ".";
        valid = false;
    }
}

void dobj::register_dobjdobj(dobjnames::DOBJ_TYPE parent,
                                                dobjnames::DOBJ_TYPE sprog)
{
    if (!valid)
        return;
    dobjdobjlist* ddl;
    ddl = jwm.get_topdobjlist()->create_dobjdobjlist(object_type, parent);
    dobjdobj* dd = 0;
    if (ddl)
        dd = ddl->register_dobjdobj(parent, sprog);
    if (!ddl || !dd)
    {
        *err_msg = "Failed to register parent data object ";
        *err_msg += jwm.get_dobjnames()->get_name(parent);
        *err_msg += " with child data object ";
        *err_msg += jwm.get_dobjnames()->get_name(sprog);
        *err_msg += ") as part of data object ";
        *err_msg += jwm.get_dobjnames()->get_name(object_type);
        *err_msg += ".";
        valid = false;
    }
}


#ifdef DOBJ_STATS
STATS_INIT(dobj)
#endif

bool dobj::first_done[dobjnames::DOBJ_LAST] = { false };

