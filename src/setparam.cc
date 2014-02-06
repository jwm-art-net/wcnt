#include "../include/setparam.h"
#include "../include/globals.h"
#include "../include/iocatconvert.h"
#include "../include/fxsparamlist.h"
#include "../include/synthmod.h"
#include "../include/dobj.h"

#include <cstdlib>

#include <iostream>

#ifdef DEBUG
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
bool set_param<synthmod::base>
    (synthmod::base* sm, int param_type, const char* value,
                                         std::ostringstream* result);

template
bool set_param<dobj::base>
    (dobj::base*, int param_type, const char* value,
                                  std::ostringstream* result);

template
void* compute<synthmod::base>
    (synthmod::base*, int param_type, void* data, int op);

template
void* compute<dobj::base>(dobj::base*, int param_type, void* data, int op);


template <typename T>
bool set_param(T* obj, int pt, const char* value,
                                       std::ostringstream* result)
{
    iocat::TYPE ioc = param::names::category(pt);
    const char* par = param::names::get(pt);

    #ifdef DEBUG
    std::cout << "\t\tset_param:\t'" << par << "' '" << value << "'" << std::endl;
    #endif

    int op = get_operator(value);
    if (op)
        value += 2; // value == ptr to txt-representation of value ;-)

    void* data = iocatconv::cstr_to_iocat(ioc, value, result);
    if (!data) {
        setpar_err("Failed to convert %s to an acceptable form for "
                "parameter %s %s.", value, par, iocatconv::get_error_msg());
        return false;
    }
    void* datatmp = data;
    if (ioc == iocat::FIX_STR) {
        fixstrparam* fsp;
        fsp = wcnt::get_fxsparamlist()->get_fix_str_param(pt);
        if (!fsp) {
        #ifdef DEBUG
            wcnt::get_fxsparamlist()->dump();
            if (!wcnt::get_fxsparamlist()->get_fix_str_param(param::FUNC))
                std::cout << "still failed manually to get param::FUNC" << std::endl;
            if (!wcnt::get_fxsparamlist()->get_fix_str_param(param::DATA_FMT))
                std::cout << "still failed manually to get param::DATA_FMT" << std::endl;
            if (!wcnt::get_fxsparamlist()->get_fix_str_param(param::WAVE_TYPE))
                std::cout << "still failed manually to get param::WAVE_TYPE" << std::endl;
        #endif

            iocatconv::destroy_iocat_data(ioc, data);
            setpar_err("No registered fixed string for '%s' parameter.", par);
            return false;
        }
        int n = fsp->get_substring_index((const char*)data);
        if (n < 0) {
            setpar_err("Parameter %s does not understand %s. Try one of "
                       "'%s' instead.", par, ((const char*)data),
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
        #ifdef DEBUG
        printf("obj:%p\n", obj);
        printf("obj username: %s\n", obj->get_username());
        printf("par:%s\n", par);
        printf("value:%s\n", value);
        #endif
        if (ioc == iocat::FIX_STR) {
            delete (int*)data;
            data = datatmp;
        }
        iocatconv::destroy_iocat_data(ioc, data);
        const char* n = obj->get_username();
        setpar_err("%s refuses to set parameter %s with value %s.",
                                        (n ? n : ""), (par ? par : ""),
                                                                value);
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
void* compute(T* obj, int pt, void* data, int op)
{
    if (!obj || pt == param::ERR_TYPE || !data || op == 0)
        return 0;
    switch(param::names::category(pt)){
        case iocat::DOUBLE:
            if (op == '/' && *(double*)data == 0)
                return 0;
            *(double*)data =
                comp((double*)obj->get_param(pt), (double*)data, op);
            break;
        case iocat::WCINT_T:
            if (op == '/' && *(wcint_t*)data == 0)
                return 0;
            *(wcint_t*)data =
                comp((wcint_t*)obj->get_param(pt), (wcint_t*)data, op);
            break;
        case iocat::SAMP_T:
            if (op == '/' && *(samp_t*)data == 0)
                return 0;
            *(samp_t*)data = comp((samp_t*)obj->get_param(pt),
                                            (samp_t*)data, op);
            break;
        default:
            return 0;
    }
    return data;
}

bool is_operator(const char* txt)
{
    // need to differentiate between a negative number and a subtraction,
    // so insist that the differentation is created by a space between
    // operator and number.
    if (*txt == '\0' || (*(txt + 1) != '\0' && *(txt + 1) != ' '))
        return false;
    return (*txt == '+' || *txt == '-' || *txt == '*' || *txt == '/');
}

int get_operator(const char* txt)
{
    if (*txt == '\0' || (*(txt + 1) != '\0' && *(txt + 1) != ' '))
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
