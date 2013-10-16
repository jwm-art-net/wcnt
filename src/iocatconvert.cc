#include "../include/iocatconvert.h"
#include "../include/meterchange.h"
#include "../include/globals.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"

#include <sstream>

#ifdef DEBUG
#define iocat_err(fmt, ... )                                \
{                                                           \
    printf("%40s:%5d %-35s\n",                              \
                    __FILE__, __LINE__, __FUNCTION__);      \
    cfmt(iocatconv::err_msg, STRBUFLEN, fmt, __VA_ARGS__);  \
}
#else
#define iocat_err(fmt, ... ) \
    cfmt(iocatconv::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif



namespace iocatconv
{
static char err_msg[STRBUFLEN] = "";

void* cstr_to_iocat(iocat::TYPE ioc, const char* cstrval,
                    std::ostringstream* result)
{
    if (!cstrval) {
        iocat_err("%s", "Expected something, got nothing!");
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
            iocat_err("%s", "Expected floating point value.");
            delete (double*)data;
            return 0;
        }
        if (result)
            *result << *(double*)data;
        break;
    case iocat::WCINT_T:
        data = new wcint_t;
        if (!(strstr >> *(wcint_t*)data)) {
            iocat_err("%s", "Expected integer value (hint:32767 max).");
            delete (wcint_t*)data;
            return 0;
        }
        if (result)
            *result << *(wcint_t*)data;
        break;
    case iocat::SAMP_T:
        data = new samp_t;
        if (!(strstr >> *(samp_t*)data)) {
            iocat_err("%s", "Expected integer value.");
            delete (samp_t*)data;
            return 0;
        }
        if (result)
            *result << *(samp_t*)data;
        break;
    case iocat::TRIG: // iocat::TRIG same as iocat::STATE
    case iocat::STATE:// except in behaviour
        data = new STATUS;
        if (strcmp(cstrval, "on") == 0)
            *(STATUS*)data = ON;
        else if (strcmp(cstrval, "off") == 0)
            *(STATUS*)data = OFF;
        else {
            iocat_err("Expected string value on or off, got %s instead.",
                                                                cstrval);
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
            iocat_err("%s", "Expected integer value for beats per bar.");
            delete (timesig*)data;
            return 0;
        }
        char ch;
        while (strstr.get(ch) && ch == ' ');
        if (ch != '/') {
            iocat_err("%s", "Expected seperator '/'.");
            delete (timesig*)data;
            return 0;
        }
        if (!(strstr >> ((timesig*)data)->beatvalue)) {
            iocat_err("%s", "Expected integer value.");
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
        data = wcnt::get_dobjlist()->get_dobj_by_name(cstrval);
        if (!data) {
            iocat_err("No data object named %s found.", cstrval);
            return 0;
        }
        if (result)
            *result << cstrval;
        break;
    case iocat::SYNTHMOD:
        data = wcnt::jwm.get_modlist()->get_synthmod_by_name(cstrval);
        if (!data) {
            iocat_err("No synthmod named %s found.", cstrval);
            return 0;
        }
        if (result)
            *result << cstrval;
        break;
    default:
        iocat_err("%s", "Invalid category.");
        return 0;
    }
    return data;
}

void destroy_iocat_data(iocat::TYPE ioc, const void* data)
{
    switch(ioc)
    {
    case iocat::DOUBLE:
        delete (double*)data;
        break;
    case iocat::WCINT_T:
        delete (wcint_t*)data;
        break;
    case iocat::SAMP_T:
        delete (samp_t*)data;
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


const char* get_error_msg()
{
    return err_msg;
}


} // namespace iocat
