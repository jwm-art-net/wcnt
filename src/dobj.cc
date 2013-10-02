#include "../include/dobj.h"
#include "../include/jwm_globals.h"
#include "../include/dobjlist.h"
#include "../include/dobjparamlist.h"
#include "../include/fxsparamlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/topdobjlist.h"


dobj::dobj(dataobj::TYPE dt) :
 object_type(dt), username(0), valid(true)
{
    #ifdef DATA_STATS
    STATS_INC
    #endif
}

dobj::~dobj()
{
    if (username)
        delete [] username;

    #ifdef DATA_STATS
    STATS_DEC
    #endif

}

bool dobj::is_named_by_user()
{
    if (dataobj::names::category(object_type) != dataobj::CAT_DEF)
    {
        dobjerr("Error in object %s type %s.",
                (username ? username : ""),
                dataobj::names::get(object_type));
        return (valid = false);
    }
    return true;
}

bool dobj::set_username(const char* un)
{
    if (!is_named_by_user()) {// should not be given username
        dobjerr("Tried to set username of object type %s, but %s "
                                        "does not require naming.",
                        dataobj::names::get(object_type));
        return (valid = false);
    }
    if (un == 0) {
        dobjerr("NULL username specified for object type %s.",
                        dataobj::names::get(object_type));
        return (valid = false);
    }
    if (strlen(un) == 0) {
        dobjerr("NULL length username specified for object type %s.",
                        dataobj::names::get(object_type));
        return (valid = false);
    }
    if (username) delete [] username;
    username = new char[strlen(un) + 1];
    strcpy(username, un);
    return true;
}

const char* dobj::get_username()
{
    if (dataobj::names::category(object_type) != dataobj::CAT_DEF)
        return 0;
    return username;
}

bool dobj::set_param(param::TYPE, const void*)
{
    dobjerr("%s %s set parameter of data object.", errors::stock::major,
                                                   errors::stock::bad);
    return 0;
}

const void* dobj::get_param(param::TYPE) const
{
    dobjerr("%s %s get parameter of data object.", errors::stock::major,
                                                   errors::stock::bad);
    return 0;
}

dobj const* dobj::add_dobj(dobj*)
{
    dobjerr("%s %s to data object", errors::stock::major, errors::stock::bad_add);
    return 0;
}

dobj* dobj::duplicate_dobj(const char* uname)
{
    (void)uname;
    dobjerr("Data object %s of type %s does not allow copies to be "
                        "made of it.", (username ? username : ""),
                        dataobj::names::get(object_type));
    return 0;
}

bool dobj::done_first() const
{
    bool r = first_done[object_type];
    first_done[object_type] = true;
    return r;
}

char dobj::err_msg[STRBUFLEN] = "";

void dobj::register_param(param::TYPE pt)
{
    if (!valid)
        return;
    if (!jwm.get_dparlist()->add_dobjparam(object_type, pt))
    {
        dobjerr("Failed to register param %s with data object type %s.",
                            param::names::get(pt),
                            dataobj::names::get(object_type));
        valid = false;
    }
}

void dobj::register_param(param::TYPE pt, const char* fixstr)
{
    if (!valid)
        return;
    dobjparam* dp = jwm.get_dparlist()->add_dobjparam(object_type, pt);
    fixstrparam* fsp = 0;
    if (dp)
        fsp = jwm.get_fxsparamlist()->add_param(fixstr, pt);
    if (!dp || !fsp)
    {
        dobjerr("Failed to register fixed string param %s (%s) "
                            "with data object type %s.",
                            param::names::get(pt), fixstr,
                            dataobj::names::get(object_type));
        valid = false;
    }
}

void dobj::register_dobjdobj(dataobj::TYPE parent,
                                                dataobj::TYPE sprog)
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
        dobjerr("Failed to register parent data object %s with child data "
                            "object %s as part of data object %s",
                            dataobj::names::get(parent),
                            dataobj::names::get(sprog),
                            dataobj::names::get(object_type));
        valid = false;
    }
}

bool dobj::validate_param(param::TYPE pt, errors::TYPE et)
{
   if (!jwm.get_dparlist()->validate(this, pt, et))
    {
        dobjerr("%s", param::names::get(pt));
        invalidate();
        return false;
    }
    return true;
}


#ifdef DATA_STATS
STATS_INIT(dobj)
#endif

bool dobj::first_done[dataobj::LAST_TYPE] = { false };

