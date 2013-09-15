#include "../include/setparam.h"
#include "../include/jwm_globals.h"
#include "../include/iocatconvert.h"
#include "../include/fxsparamlist.h"
#include "../include/synthmod.h"
#include "../include/dobj.h"

#include <cstdlib>

#include <iostream>

#ifdef DEBUG_MSG
#define setpar_err(fmt, ... )                           \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(setpar::err_msg, STRBUFLEN, fmt, __VA_ARGS__); \
}
#else
#define setpar_err(fmt, ... ) \
    cfmt(setpar::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif


namespace setpar
{

static char err_msg[STRBUFLEN] = "";

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
    char const* parname = jwm.get_paramnames()->get_name(pt);
    if (pt != paramnames::STR_UNNAMED && pt != paramnames::STR_LIST)
    {
        if (strcmp(param, parname) != 0) {
            setpar_err("Expected %s got %s instead. %s",
                            parname, param,
                    ((jwm.get_paramnames()->get_type(param)
                                                == paramnames::FIRST
                        && (time(0) % 8) == 0)
                                ? ". Just can't get the staff these days."
                                : ""));
            return false;
        }
    }
    iocat::IOCAT ioc = jwm.get_paramnames()->get_category(pt);
    int op = get_operator(value);
    if (op)
        value += 2; // value == ptr to txt-representation of value ;-)

    void* data = iocatconv::cstr_to_iocat(ioc, value, result);
    if (!data) {
        setpar_err("Failed to convert %s to an acceptable form for "
                "parameter %s %s.", value, param,
                                            iocatconv::get_error_msg());
        return false;
    }
    void* datatmp = data;
    if (ioc == iocat::FIX_STR) {
        fixstrparam* fsp;
        fsp = jwm.get_fxsparamlist()->get_fix_str_param(pt);
        if (!fsp) {
            iocatconv::destroy_iocat_data(ioc, data);
            setpar_err("No registered fixed string for %s parameter.",
                        param);
            return false;
        }
        int n = fsp->get_substring_index((char const*)data);
        if (n < 0) {
            setpar_err("Parameter %s does not understand %s. Try one of "
                       "'%s' instead.", param, ((char const*)data),
                                                fsp->get_string_list());
            iocatconv::destroy_iocat_data(ioc, data);
            return false;
        }
        data = new int(n);
    }
    if (op) {
        if (!compute(obj, pt, data, op)) {
            setpar_err("Failed to compute parameter value using operator"
                                                           " '%s'.", op);
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
        setpar_err("Module %s refuses to set parameter %s with value %s.",
                                        obj->get_username(), param, value);
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
    switch(jwm.get_paramnames()->get_category(pt)){
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


const char* get_error_msg()
{
    return err_msg;
}

} // namespace setpar

