#include "../include/setparam.h"
#include "../include/jwm_globals.h"
#include "../include/iocatconvert.h"
#include "../include/fxsparamlist.h"
#include "../include/synthmod.h"
#include "../include/dobj.h"

#include <cstdlib>

#include <iostream>

namespace setpar
{

template
bool set_param<synthmod>
    (synthmod* sm, const char* param, paramnames::PAR_TYPE pt,
        const char* value, std::ostringstream* result);

template
bool set_param<dobj>
    (dobj*, const char* param, paramnames::PAR_TYPE pt,
        const char* value, std::ostringstream* result);

template
void* compute<synthmod>
    (synthmod*, paramnames::PAR_TYPE pt, void* data, int op);

template
void* compute<dobj>(dobj*, paramnames::PAR_TYPE pt, void* data, int op);

template <typename T>
bool set_param(T* obj, const char* param, paramnames::PAR_TYPE pt,
    const char* value, std::ostringstream* result)
{
    char const* parname = paramnames::get_name(pt);
    if (pt != paramnames::STR_UNNAMED && pt != paramnames::STR_LIST)
    {
        if (strcmp(param, parname) != 0) {
            err_msg = ", expected ";
            err_msg += parname;
            err_msg += " got ";
            err_msg += param;
            err_msg += " instead";
            if (paramnames::get_type(param)
                == paramnames::FIRST && (time(0) % 8) == 0)
            {
                err_msg += ". just can't get the staff these days...";
            }
            return false;
        }
    }
    iocat::IOCAT ioc = paramnames::get_category(pt);
    int op = get_operator(value);
    if (op)
        value += 2; // value == ptr to txt-representation of value ;-)

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
    if (op) {
        if (!compute(obj, pt, data, op)) {
            err_msg  = ", well that's truly screwed it my friend.";
            err_msg += " can't perform operation on parameter using ";
            err_msg += "operator ";
            err_msg += op;
            err_msg += " hopefully i can be a little clearer soon...";
            iocatconv::destroy_iocat_data(ioc, data);
            return false;
        }
    }
    if (!obj->set_param(pt, data)) {
        if (ioc == iocat::FIX_STR) {
            delete (int*)data;
            data = datatmp;
        }
        iocatconv::destroy_iocat_data(ioc, data);
        err_msg = "module ";
        err_msg += obj->get_username();
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

template <typename T>
void* compute(T* obj, paramnames::PAR_TYPE pt, void* data, int op)
{
    if (!obj || pt == paramnames::FIRST || !data || op == 0)
        return 0;
    switch(paramnames::get_category(pt)){
        case iocat::DOUBLE:
            if (op == '/' && *(double*)data == 0)
                return 0;
            *(double*)data =
                comp((double*)obj->get_param(pt), (double*)data, op);
            break;
        case iocat::SHORT:
            if (op == '/' && *(short*)data == 0)
                return 0;
            *(short*)data =
                comp((short*)obj->get_param(pt), (short*)data, op);
            break;
        case iocat::ULONG:
            if (op == '/' && *(unsigned long*)data == 0)
                return 0;
            *(unsigned long*)data =
                comp((unsigned long*)obj->get_param(pt),
                     (unsigned long*)data, op);
            break;
        default:
            return 0;
    }
    return data;
}

bool is_operator(const char* txt)
{
    if (strcmp(txt,"+") == 0) return true;
    if (strcmp(txt,"-") == 0) return true;
    if (strcmp(txt,"*") == 0) return true;
    if (strcmp(txt,"/") == 0) return true;
    return false;
}

int get_operator(const char* txt)
{
    if (*(txt+1)!=' ')
        return 0;
    switch(*txt){
        case '+':
        case '-':
        case '*':
        case '/':
            return *txt;
        default:
            return 0;
    }
}


std::string err_msg;

} // namespace setpar

