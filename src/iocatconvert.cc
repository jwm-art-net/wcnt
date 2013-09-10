#ifndef IOCATCONVERT_H
#include "../include/iocatconvert.h"

using namespace std;

namespace iocatconv
{

void* cstr_to_iocat(IOCAT iocat, char const* cstrval, 
                    ostringstream* result)
{
    if (!cstrval) {
        err_msg = ", expected something, got nothing";
        return 0;
    }
    stringstream strstr(cstrval);
    char* cstr = 0;
    void* data = 0;
    switch(iocat)
    {
    case CAT_DOUBLE:
        data = new double;
        if (!(strstr >> *(double*)data)) {
            err_msg = ", expected floating point value";
            delete (double*)data;
            return 0;
        }
        if (result)
            *result << *(double*)data;
        break;
    case CAT_SHORT:
        data = new short;
        if (!(strstr >> *(short*)data)) {
            err_msg = ", expected integer value (hint:32767 max)";
            delete (short*)data;
            return 0;
        }
        if (result)
            *result << *(short*)data;
        break;
    case CAT_ULONG:
        data = new unsigned long;
        if (!(strstr >> *(unsigned long*)data)) {
            err_msg = ", expected unsigned long integer value";
            delete (unsigned long*)data;
            return 0;
        }
        if (result)
            *result << *(unsigned long*)data;
        break;
    case CAT_TRIG: // CAT_TRIG same as CAT_STATE
    case CAT_STATE:// except in behaviour
        data = new STATUS;
        if (strcmp(cstrval, "on") == 0)
            *(STATUS*)data = ON;
        else if (strcmp(cstrval, "off") == 0)
            *(STATUS*)data = OFF;
        else {
            err_msg = ", expected string value on or off, got ";
            err_msg += cstrval;
            err_msg += " instead";
            delete (STATUS*)data;
            return 0;
        }
        if (result)
            *result << cstrval;
        break;
    case CAT_STRING:
    case CAT_FIX_STR:
        cstr = new char[strlen(cstrval) + 1];
        strcpy(cstr, cstrval);
        data = cstr;
        if (result)
            *result << cstrval;
        break;
    case CAT_METER:
        data = new timesig;
        if (!(strstr >> ((timesig*)data)->beatsperbar)) {
            err_msg = ", expected integer value for beats per bar";
            delete (timesig*)data;
            return 0;
        }
        char ch;
        while (strstr.get(ch) && ch == ' ');
        if (ch != '/') {
            err_msg = ", expected seperator /";
            delete (timesig*)data;
            return 0;
        }
        if (!(strstr >> ((timesig*)data)->beatvalue)) {
            err_msg = ", expected integer value";
            delete (timesig*)data;
            return 0;
        }
        if (result) {
            *result << ((timesig*)data)->beatsperbar;
            *result << "/";
            *result << ((timesig*)data)->beatvalue;
        }
        break;
    case CAT_DOBJ:
        data = dobj::get_dobjlist()->get_dobj_by_name(cstrval);
        if (!data) {
            err_msg = ", no data object named ";
            err_msg += cstrval;
            err_msg += " found";
            return 0;
        }
        if (result)
            *result << cstrval;
        break;
    case CAT_SYNTHMOD:
        data =
         synthmod::get_modlist()->get_synthmod_by_name(cstrval);
        if (!data) {
            err_msg = ", no synth module found named ";
            err_msg += cstrval;
            return 0;
        }
        if (result)
            *result << cstrval;
        break;
    default:
        err_msg = "invalid category";
        return 0;
    }
    return data;
}

void destroy_iocat_data(IOCAT iocat, void* data)
{
    switch(iocat)
    {
    case CAT_DOUBLE:
        delete (double*)data;
        break;
    case CAT_SHORT:
        delete (short*)data;
        break;
    case CAT_ULONG:
        delete (unsigned long*)data;
        break;
    case CAT_TRIG:  // cat_trig same as
    case CAT_STATE: // cat_state
        delete (STATUS*)data;
        break;
    case CAT_STRING:
    case CAT_FIX_STR:
        delete [] (char*)data;
        break;
    case CAT_METER:
        delete (timesig*)data;
        break;
    case CAT_SYNTHMOD:	// don't delete as it's not created
    case CAT_DOBJ:		// in read_iocat_value(...)
    default:
        break;
    }
}

std::string err_msg;

} // namespace iocat
#endif
