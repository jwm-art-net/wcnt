#include "../include/dobj.h"
#include "../include/globals.h"
#include "../include/dobjlist.h"
#include "../include/fxsparamlist.h"
#include "../include/ui_dobjitem.h"

namespace dobj
{

 base::base(TYPE dt) :
  object_type(dt), username(0), valid(true)
 {
    #ifdef DATA_STATS
    STATS_INC
    #endif
 }

 base::~base()
 {
    if (username)
        delete [] username;

    #ifdef DATA_STATS
    STATS_DEC
    #endif
 }

 void base::ui_register()
 {
    if (!first_done[object_type]) {
        register_ui();
        first_done[object_type] = true;
    }
 }

 bool base::is_named_by_user()
 {
    if (names::category(object_type) != CAT_DEF)
    {
        dobjerr("Error in object %s type %s.",
                (username ? username : ""), names::get(object_type));
        return (valid = false);
    }
    return true;
 }

 bool base::set_username(const char* un)
 {
    if (!is_named_by_user()) {// should not be given username
        dobjerr("Tried to set username of object type %s, but %s "
                "does not require naming.", names::get(object_type));
        return (valid = false);
    }
    if (un == 0) {
        dobjerr("NULL username specified for object type %s.",
                                            names::get(object_type));
        return (valid = false);
    }
    if (strlen(un) == 0) {
        dobjerr("NULL length username specified for object type %s.",
                                            names::get(object_type));
        return (valid = false);
    }
    if (username) delete [] username;
    username = new char[strlen(un) + 1];
    strcpy(username, un);
    return true;
 }

 const char* base::get_username()
 {
    if (names::category(object_type) != CAT_DEF)
        return 0;
    return username;
 }

 bool base::set_param(param::TYPE, const void*)
 {
    dobjerr("%s %s set parameter of data object.", errors::stock::major,
                                                   errors::stock::bad);
    return 0;
 }

 const void* base::get_param(param::TYPE) const
 {
    dobjerr("%s %s get parameter of data object.", errors::stock::major,
                                                   errors::stock::bad);
    return 0;
 }

 const base* base::add_dobj(base*)
 {
    dobjerr("%s %s to data object", errors::stock::major,
                                    errors::stock::bad_add);
    return 0;
 }

 base* base::duplicate_dobj(const char* uname)
 {
    (void)uname;
    dobjerr("Data object %s of type %s does not allow copies to be "
                        "made of it.", (username ? username : ""),
                                            names::get(object_type));
    return 0;
 }

 char base::err_msg[STRBUFLEN] = "";

 void base::register_param(param::TYPE pt)
 {
    if (!valid)
        return;

    if (!wcnt::get_ui_dobjitem_list()->add_item(object_type, pt)) {
        dobjerr("Failed to register param %s with data object type %s.",
                            param::names::get(pt), names::get(object_type));
        valid = false;
    }
 }

 void base::register_param(param::TYPE pt, const char* fixstr)
 {
    if (!valid)
        return;

    if (!wcnt::get_ui_dobjitem_list()->add_item(object_type, pt, fixstr)) {
        dobjerr("Failed to register fixed string param %s (%s) "
                            "with data object type %s.",
                            param::names::get(pt), fixstr,
                            names::get(object_type));
        valid = false;
    }
 }

 void base::register_dobj(TYPE parent, TYPE sprog)
 {
    if (!valid)
        return;
    if (!wcnt::get_ui_dobjitem_list()->add_item(object_type, parent, sprog))
    {
        dobjerr("Failed to register parent data object %s with child data "
                "object %s as part of data object %s", names::get(parent),
                                names::get(sprog), names::get(object_type));
        valid = false;
    }
 }

 bool base::validate_param(param::TYPE pt, errors::TYPE et)
 {
   if (!wcnt::get_ui_dobjitem_list()->validate(this, pt, et))
    {
        dobjerr("%s", param::names::get(pt));
        invalidate();
        return false;
    }
    return true;
 }


 #ifdef DATA_STATS
 STATS_INIT(base)
 #endif

 bool base::first_done[LAST_TYPE] = { false };

}; //namespace dobj
