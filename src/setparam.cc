#ifndef SETPARAM_H
#include "../include/setparam.h"
#include "../include/jwm_globals.h"
#include "../include/iocatconvert.h"
#include "../include/fxsparamlist.h"
#include "../include/synthmod.h"
#include "../include/dobj.h"

#include <stdlib.h>

namespace setpar
{

bool set_mod_param(synthmod* module, const char* param,
 paramnames::PAR_TYPE pt, const char* value, std::ostringstream* result)
{
    char const* parname = jwm.get_paramnames()->get_name(pt);
    if (strcmp(param, parname) != 0) {
        err_msg = ", expected ";
        err_msg += parname;
        err_msg += " got ";
        err_msg += param;
        err_msg += " instead";
        if (jwm.get_paramnames()->get_type(param)
            == paramnames::FIRST && (time(0) % 8) == 0)
        {
            err_msg += ". just can't get the staff these days...";
        }
        return false;
    }
    iocat::IOCAT ioc = jwm.get_paramnames()->get_category(pt);
    void* data = iocatconv::cstr_to_iocat(ioc, value, result);
    if (!data) {
        err_msg = "\nan error occurred trying to convert ";
        err_msg += value;
        err_msg += " to an acceptable form for parameter ";
        err_msg += param;
        err_msg += iocatconv::err_msg;
        return false;
    }
    void* datatmp = data;
    if (ioc == iocat::FIX_STR) {
        fixstrparam* fsp;
        fsp = jwm.get_fxsparamlist()->get_fix_str_param(pt);
        if (!fsp) {
            iocatconv::destroy_iocat_data(ioc, data);
            err_msg = ", no registered fixed string for ";
            err_msg += param;
            err_msg += " parameter";
            return false;
        }
        int n = fsp->get_substring_index((char const*)data);
        if (n < 0) {
            err_msg = ", parameter ";
            err_msg += param;
            err_msg += " does not understand ";
            err_msg += ((char const*)data);
            err_msg += ". try one of '";
            err_msg += fsp->get_string_list();
            err_msg += "' instead";
            iocatconv::destroy_iocat_data(ioc, data);
            return false;
        }
        data = new int(n);
    }
    if (!module->set_param(pt, data)) {
        if (ioc == iocat::FIX_STR) {
            delete (int*)data;
            data = datatmp;
        }
        iocatconv::destroy_iocat_data(ioc, data);
        err_msg = "module ";
        err_msg += module->get_username();
        err_msg += " refuses to set parameter ";
        err_msg += param;
        err_msg += " with value ";
        err_msg += value;
        err_msg += ". Either ";
        err_msg += param;
        err_msg += " is the wrong type of thing expected, or...";
        return false;
    }
    if (ioc == iocat::FIX_STR) {
        delete (int*)data;
        data = datatmp;
    }
    iocatconv::destroy_iocat_data(ioc, data);
    return true;
}

bool set_dobj_param(dobj* dbj, const char* param,
 paramnames::PAR_TYPE pt, const char* value, std::ostringstream* result)
{
    if (pt != paramnames::STR_UNNAMED
        && pt != paramnames::STR_LIST)
    {
        char const* parname = jwm.get_paramnames()->get_name(pt);
        if (strcmp(param, parname) != 0) {
            err_msg = ", expected ";
            err_msg += parname;
            err_msg += " got ";
            err_msg += param;
            err_msg += " instead";
            if (jwm.get_paramnames()->get_type(param)
                == paramnames::FIRST && (time(0) % 8) == 0)
            {
                err_msg += ". just can't get the staff these days...";
            }
            return false;
        }
    }
    iocat::IOCAT ioc = jwm.get_paramnames()->get_category(pt);
    void* data = iocatconv::cstr_to_iocat(ioc, value, result);
    if (!data) {
        err_msg = ", an error occurred trying to convert ";
        err_msg += value;
        err_msg += " to an acceptable form for parameter ";
        err_msg += param;
        err_msg += iocatconv::err_msg;
        err_msg += ".";
        return false;
    }
    void* datatmp = data;
    if (ioc == iocat::FIX_STR) {
        fixstrparam* fsp;
        fsp = jwm.get_fxsparamlist()->get_fix_str_param(pt);
        if (!fsp) {
            iocatconv::destroy_iocat_data(ioc, data);
            err_msg = ", no registered fixed string for ";
            err_msg += param;
            err_msg += " parameter.";
            return false;
        }
        int n = fsp->get_substring_index((char const*)data);
        if (n < 0) {
            err_msg = ", parameter ";
            err_msg += param;
            err_msg += " does not understand ";
            err_msg += ((char const*)data);
            err_msg += ". try one of '";
            err_msg += fsp->get_string_list();
            err_msg += "' instead.";
            iocatconv::destroy_iocat_data(ioc, data);
            return false;
        }
        data = new int(n);
    }
    if (!dbj->set_param(pt, data)) {
        if (ioc == iocat::FIX_STR) {
            delete (int*)data;
            data = datatmp;
        }
        iocatconv::destroy_iocat_data(ioc, data);
        err_msg = ". data object ";
        err_msg += jwm.get_dobjnames()->get_name(dbj->get_object_type());
        //get_username();
        err_msg += " refuses to set parameter ";
        err_msg += param;
        err_msg += " with value ";
        err_msg += value;
        err_msg += ". I suspect that ";
        err_msg += value;
        err_msg += " is probably not compatible with parameter ";
        err_msg += param;
        err_msg += ".";
        err_msg += *dobj::get_error_msg();
        return false;
    }
    if (ioc == iocat::FIX_STR) {
        delete (int*)data;
        data = datatmp;
    }
    iocatconv::destroy_iocat_data(ioc, data);
    return true;
}



std::string err_msg;

} // namespace setpar

#endif
