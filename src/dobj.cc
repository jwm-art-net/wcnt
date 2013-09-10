#ifndef DOBJ_H
#include "../include/dobj.h"

dobj::dobj(dobjnames::DOBJ_TYPE dt) :
 object_type(dt), username(0), valid(true)
{
    *err_msg = ""; // hmmm
    #ifdef SHOW_DOBJ_COUNT
    dobjs_created_count++;
    dobjs_count++;
    dobjs_max_count = (dobjs_count > dobjs_max_count)
                        ? dobjs_count : dobjs_max_count;
    #endif
    #ifdef SHOW_DOBJ_NAMES
    cout << "\nCreating object of type " << dobj_names->get_name(dt);
    #endif
}

dobj::~dobj()
{
    #ifdef SHOW_DOBJ_COUNT
    dobjs_destroyed_count++;
    dobjs_count--;
    #endif
    #ifdef SHOW_DOBJ_NAMES
    cout << "\nDestroying object of type ";
    cout << dobj_names->get_name(object_type);
    if (object_type > dobjnames::DOBJ_DEFS && username)
        cout << " (" << username << ")";
    #endif
    if (username)
        delete [] username;
}

bool dobj::is_named_by_user()
{
    if (get_dobjnames()->check_type(object_type) == dobjnames::DOBJ_FIRST)
    {
        *err_msg = "\nError in object ";
        if (username) *err_msg += username;
        *err_msg += " type ";
        *err_msg += dobj_names->get_name(object_type);
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
        *err_msg += dobj_names->get_name(object_type);
        *err_msg += " - does not required naming";
        return (valid = false);
    }
    if (un == 0) {
        *err_msg = "\nNULL username specified for object type ";
        *err_msg += dobj_names->get_name(object_type);
        return (valid = false);
    }
    if (strlen(un) == 0) {
        *err_msg = "\nNULL length username specified for object type ";
        *err_msg += dobj_names->get_name(object_type);
        return (valid = false);
    }
    if (username) delete [] username;
    username = new char[strlen(un) + 1];
    strcpy(username, un);
    return true;
}

char const* dobj::get_username()
{
    if (get_dobjnames()->check_type(object_type) == dobjnames::DOBJ_FIRST)
        return 0;
    return username;
}

bool dobj::set_param(paramnames::PAR_TYPE, void*)
{
    *err_msg = 
     "\n***programmer error***\nattempt made to set parameter data";
    *err_msg += "\nof data object lacking capability to set them.";
    return 0;
}

void const* dobj::get_param(paramnames::PAR_TYPE)
{
    *err_msg = 
     "\n***programmer error***\nattempt made to get parameter data";
    *err_msg += "\nfrom data object lacking capability to provide them.";
    return 0;
}

dobj const* dobj::add_dobj(dobj*)
{
    *err_msg = 
     "\n***programmer error***\nattempt made to add data object to";
    *err_msg +=	"\ndata object lacking ability to accept them";
    return 0;
}

dobj* dobj::duplicate_dobj(const char* uname)
{
    *err_msg = "from:\ndata object ";
    if (username) *err_msg += username;
    *err_msg += " of data type ";
    *err_msg += dobj_names->get_name(object_type);
    *err_msg += " does not allow copies to be made of it.";
    return 0;
}

char* dobj::path = 0;
string*  dobj::err_msg = 0;
iocat_names* dobj::iocatnames = 0;
dobjnames* dobj::dobj_names = 0;
dobjlist* dobj::dobj_list = 0;
dobjparamlist* dobj::dobjpar_list = 0;
paramnames* dobj::par_names = 0;
topdobjlist* dobj::top_dobjlist = 0;
fxsparamlist* dobj::fxsparlist = 0;
#ifdef SHOW_DOBJ_COUNT
long dobj::dobjs_created_count = 0;
long dobj::dobjs_destroyed_count = 0;
long dobj::dobjs_count = 0;
long dobj::dobjs_max_count = 0;
#endif

#endif
