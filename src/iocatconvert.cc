#ifndef IOCATCONVERT_H
#include "../include/iocatconvert.h"
#include "../include/meterchange.h"
#include "../include/jwm_globals.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"

#include <sstream>

namespace iocatconv
{

void* cstr_to_iocat(iocat::IOCAT ioc, char const* cstrval, 
                    std::ostringstream* result)
{
    if (!cstrval) {
        err_msg = ", expected something, got nothing";
        return 0;
    }
    std::stringstream strstr(cstrval);
    char* cstr = 0;
    void* data = 0;
    switch(ioc)
    {
    case iocat::DOUBLE:
        data = new double;
        if (!(strstr >> *(double*)data)) {
            err_msg = ", expected floating point value";
            delete (double*)data;
            return 0;
        }
        if (result)
            *result << *(double*)data;
        break;
    case iocat::SHORT:
        data = new short;
        if (!(strstr >> *(short*)data)) {
            err_msg = ", expected integer value (hint:32767 max)";
            delete (short*)data;
            return 0;
        }
        if (result)
            *result << *(short*)data;
        break;
    case iocat::ULONG:
        data = new unsigned long;
        if (!(strstr >> *(unsigned long*)data)) {
            err_msg = ", expected unsigned long integer value";
            delete (unsigned long*)data;
            return 0;
        }
        if (result)
            *result << *(unsigned long*)data;
        break;
    case iocat::TRIG: // iocat::TRIG same as iocat::STATE
    case iocat::STATE:// except in behaviour
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
    case iocat::STRING:
    case iocat::FIX_STR:
        cstr = new char[strlen(cstrval) + 1];
        strcpy(cstr, cstrval);
        data = cstr;
        if (result)
            *result << cstrval;
        break;
    case iocat::METER:
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
    case iocat::DOBJ:
        data = jwm.get_dobjlist()->get_dobj_by_name(cstrval);
        if (!data) {
            err_msg = ", no data object named ";
            err_msg += cstrval;
            err_msg += " found";
            return 0;
        }
        if (result)
            *result << cstrval;
        break;
    case iocat::SYNTHMOD:
        data = jwm.get_modlist()->get_synthmod_by_name(cstrval);
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

void destroy_iocat_data(iocat::IOCAT ioc, void* data)
{
    switch(ioc)
    {
    case iocat::DOUBLE:
        delete (double*)data;
        break;
    case iocat::SHORT:
        delete (short*)data;
        break;
    case iocat::ULONG:
        delete (unsigned long*)data;
        break;
    case iocat::TRIG:  // iocat::trig same as
    case iocat::STATE: // iocat::state
        delete (STATUS*)data;
        break;
    case iocat::STRING:
    case iocat::FIX_STR:
        delete [] (char*)data;
        break;
    case iocat::METER:
        delete (timesig*)data;
        break;
    case iocat::SYNTHMOD:	// don't delete as it's not created
    case iocat::DOBJ:		// in read_ioiocat::value(...)
    default:
        break;
    }
}

std::string err_msg;

} // namespace iocat
#endif
