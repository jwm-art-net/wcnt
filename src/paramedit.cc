#include "../include/paramedit.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"
#include "../include/modparamlist.h"
#include "../include/dobjparamlist.h"
#include "../include/setparam.h"
#include "../include/jwm_globals.h"

#include <iostream>


paramedit::paramedit() :
 dobj(dobjnames::SIN_EDIT_PARAM),
 name(0), parstr(0)
{
    init_first();
}

paramedit::~paramedit()
{
    if (name)
        delete [] name;
    if (parstr)
        delete [] parstr;
}

bool paramedit::set_name(const char* n)
{
    synthmod* sm = jwm.get_modlist()->get_synthmod_by_name(n);
    dobj* dbj = jwm.get_dobjlist()->get_dobj_by_name(n);
    if (!sm && !dbj)
        return false;
    if (name)
        delete [] name;
    name = new char[strlen(n) + 1];
    strcpy(name, n);
    return true;
}

void paramedit::set_parstr(const char* n)
{
    if (parstr)
        delete [] parstr;
    parstr = new char[strlen(n) + 1];
    strcpy(parstr, n);
    return;
}

bool paramedit::do_param_edits()
{
    synthmod* sm = jwm.get_modlist()->get_synthmod_by_name(name);
    dobj* dbj = jwm.get_dobjlist()->get_dobj_by_name(name);
    if (sm && dbj) {
        dobjerr("A data object and module share the username %s. Cannot "
                    "edit parameters %s", name, parstr);
        invalidate();
        return false;
    }
    if (!sm && !dbj) {
        dobjerr("There are no data objects or modules named %s. Cannot "
                    "edit parameters.", name);
        invalidate();
        return false;
    }
    std::stringstream strm;
    std::string parname;
    std::string valstr;
    strm << parstr;
    strm >> parname;
    do {
        strm >> valstr;
        bool isop = setpar::is_operator(valstr.c_str());
        if (isop) {
            std::string n;
            strm >> n;
            valstr += " " + n;
        }
        if (sm) {
            if (!mod_param_edit(sm, parname.c_str(), valstr.c_str())) {
                return false;
            }
        }
        else {
            if (!dobj_param_edit(dbj, parname.c_str(), valstr.c_str())) {
                return false;
            }
        }
        if (jwm.is_verbose())
            std::cout << "\n    " << parname << " " << valstr;
        strm >> parname;
    }while (!strm.eof());
    return true;
}

bool paramedit::mod_param_edit(synthmod* module, const char* parname,
                               const char* valstr)
{
    synthmodnames::SYNTH_MOD_TYPE smt = module->get_module_type();
    modparamlist::linkedlist*
        parlist = new_list_of_by(jwm.get_paramlist(),smt);

    modparam* mp = parlist->goto_first();
    paramnames::PAR_TYPE pt = paramnames::FIRST;
    bool confused = false;
    while(mp) {
        paramnames::PAR_TYPE mpt = mp->get_paramtype();
        char const* mparname = jwm.get_paramnames()->get_name(mpt);
        if (strcmp(parname, mparname) == 0) {
            if (pt != paramnames::FIRST)
                confused = true;
            pt = mpt;
        }
        mp = parlist->goto_next();
    }
    if (confused) {
        dobjerr("There seems to be more than one parameter named %s "
                                            "- this is a bug.", parname);
        invalidate();
        delete parlist;
        return false;
    }
    if (pt == paramnames::FIRST) {
        dobjerr("Module %s does not have any parameter named %s.",
                                        module->get_username(), parname);
        invalidate();
        delete parlist;
        return false;
    }
    if (!setpar::set_param(module, parname, pt, valstr, 0)) {
        dobjerr("%s", setpar::get_error_msg());
        invalidate();
        delete parlist;
        return false;
    }
    delete parlist;
    return true;
}

bool paramedit::dobj_param_edit(dobj* dobject, const char* parname, 
                                const char* valstr)
{
    dobjnames::DOBJ_TYPE dt = dobject->get_object_type();
    dobjparamlist::linkedlist*
        parlist = new_list_of_by(jwm.get_dparlist(), dt);

    dobjparam* dp = parlist->goto_first();
    paramnames::PAR_TYPE pt = paramnames::FIRST;
    bool confused = false;
    while(dp) {
        paramnames::PAR_TYPE dpt = dp->get_partype();
        char const* mparname = jwm.get_paramnames()->get_name(dpt);
        if (strcmp(parname, mparname) == 0) {
            if (pt != paramnames::FIRST)
                confused = true;
            pt = dpt;
        }
        dp = parlist->goto_next();
    }
    if (confused) {
        dobjerr("There seems to be more than one parameter named %s "
                                            "- this is a bug.", parname);
        invalidate();
        delete parlist;
        return false;
    }
    if (pt == paramnames::FIRST) {
        dobjerr("Data object %s does not have any parameter named %s.",
                                        dobject->get_username(), parname);
        invalidate();
        delete parlist;
        return false;
    }
    if (!setpar::set_param(dobject, parname, pt, valstr, 0)) {
        dobjerr("%s", setpar::get_error_msg());
        invalidate();
        delete parlist;
        return false;
    }
    delete parlist;
    return true;
}

bool paramedit::set_param(paramnames::PAR_TYPE dt, void* data)
{
    switch(dt)
    {
    case paramnames::STR_UNNAMED:
        if (!set_name((char*)data)) {
            dobjerr("There are no data objects or modules named %s "
                    "cannot edit parameters.", (char*)data);
            invalidate();
            return false;
        }
        return true;
    case paramnames::STR_LIST:
        set_parstr((char*)data);
        return true;
    default:
        return false;
    }
}

void const* paramedit::get_param(paramnames::PAR_TYPE dt) const
{
    switch(dt)
    {
        case paramnames::STR_UNNAMED:   return get_name();
        case paramnames::STR_LIST:      return get_parstr();
        default: return 0;
    }
}

void paramedit::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::STR_UNNAMED);
    register_param(paramnames::STR_LIST);
}



