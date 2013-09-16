#include "../include/dobj.h"
#include "../include/jwm_globals.h"
#include "../include/dobjlist.h"


dobj::dobj(dobjnames::DOBJ_TYPE dt) :
 object_type(dt), username(0), valid(true)
{
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
        dobjerr("Error in object %s type %s.",
                (username ? username : ""),
                jwm.get_dobjnames()->get_name(object_type));
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
        dobjerr("Tried to set username of object type %s, but %s "
                                        "does not require naming.",
                        jwm.get_dobjnames()->get_name(object_type));
        return (valid = false);
    }
    if (un == 0) {
        dobjerr("NULL username specified for object type %s.",
                        jwm.get_dobjnames()->get_name(object_type));
        return (valid = false);
    }
    if (strlen(un) == 0) {
        dobjerr("NULL length username specified for object type %s.",
                        jwm.get_dobjnames()->get_name(object_type));
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
    dobjerr("%s", "*** MAJOR ERROR *** Bad attempt made to set parameter "
                                                       "of data object.");
    return 0;
}

void const* dobj::get_param(paramnames::PAR_TYPE) const
{
    dobjerr("%s", "*** MAJOR ERROR *** Bad attempt made to get parameter "
                                                       "of data object.");
    return 0;
}

dobj const* dobj::add_dobj(dobj*)
{
    dobjerr("%s", "*** MAJOR ERROR *** Bad attempt made to add data object"
                                                      " to data object.");
    return 0;
}

dobj* dobj::duplicate_dobj(const char* uname)
{
    dobjerr("Data object %s of type %s does not allow copies to be "
                        "made of it.", (username ? username : ""),
                        jwm.get_dobjnames()->get_name(object_type));
    return 0;
}

bool dobj::done_params()
{
    bool r = params_done[object_type];
    params_done[object_type] = true;
    return r;
}

char dobj::err_msg[STRBUFLEN] = "";

#ifdef DOBJ_STATS
STATS_INIT(dobj)
#endif

bool dobj::params_done[dobjnames::DOBJ_LAST] = { false };

