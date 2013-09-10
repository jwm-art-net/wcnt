#ifndef PARAMEDIT_H
#include "../include/paramedit.h"

paramedit::paramedit() :
 dobj(dobjnames::SIN_EDIT_PARAM),
 name(0), parstr(0)
{
    create_params();
}

paramedit::~paramedit()
{
    if (name)
        delete [] name;
    if (parstr)
        delete [] parstr;
}

void paramedit::set_name(const char* n)
{
    if (name)
        delete [] name;
    name = new char[strlen(n) + 1];
    strcpy(name, n);
    return;
}

void paramedit::set_parstr(const char* n)
{
    if (parstr)
        delete [] parstr;
    parstr = new char[strlen(n) + 1];
    strcpy(parstr, n);
    return;
}

bool paramedit::do_param_edits(bool verbose)
{
    synthmod* sm = synthmod::get_modlist()->get_synthmod_by_name(name);
    dobj* dbj = dobj::get_dobjlist()->get_dobj_by_name(name);
    if (sm && dbj) {
        *err_msg ="\na data object and module share username ";
        *err_msg += name;
        *err_msg += ". will not edit parameters ";
        *err_msg += parstr;
        invalidate();
        return false;
    }
    if (!sm && !dbj) {
        *err_msg = "\nthere are no data objects or modules named ";
        *err_msg += name;
        *err_msg += ". cannot edit parameters of non-existant entity.";
        invalidate();
        return false;
    }
    stringstream strm;
    string parname;
    string valstr;
    strm << parstr;
    strm >> parname;
    do {
        strm >> valstr;
        if (sm) {
            if (!mod_param_edit(sm, parname.c_str(), valstr.c_str())) {
                *err_msg += "...modshit happens in userland...";
                return false;
            }
        }
        else {
            if (!dobj_param_edit(dbj, parname.c_str(), valstr.c_str())) {
                *err_msg += "...dobjshit happens in userland...";
                return false;
            }
        }
        if (verbose)
            cout << "\n    " << parname << " " << valstr;
        strm >> parname;
    }while (!strm.eof());
    return true;
}

bool paramedit::mod_param_edit(synthmod* module, const char* parname, 
                               const char* valstr)
{
    synthmodnames::SYNTH_MOD_TYPE smt = module->get_module_type();
    modparamlist* parlist = synthmod::get_paramlist()->
        get_paramlist_for_moduletype(smt);
    modparam* mp = parlist->goto_first();
    paramnames::PAR_TYPE pt = paramnames::PAR_FIRST;
    bool confused = false;
    while(mp) {
        paramnames::PAR_TYPE mpt = mp->get_paramtype();
        char const* mparname = get_paramnames()->get_name(mpt);
        if (strcmp(parname, mparname) == 0) {
            if (pt != paramnames::PAR_FIRST)
                confused = true;
            pt = mpt;
        }
        mp = parlist->goto_next();
    }
    if (confused) {
        *err_msg = " there seems to be more than one parameter named ";
        *err_msg += parname;
        *err_msg += ". cannot choose. please inform wcnt developer";
        invalidate();
        delete parlist;
        return false;
    }
    if (pt == paramnames::PAR_FIRST) {
        *err_msg = "\nmodule ";
        *err_msg += module->get_username();
        *err_msg += " does not have any parameter named ";
        *err_msg += parname;
        invalidate();
        delete parlist;
        return false;
    }
    if (!setpar::set_mod_param(module, parname, pt, valstr, 0)) {
        *err_msg = "\n";
        *err_msg += setpar::err_msg;
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
    dobjparamlist* parlist = dobj::get_dparlist()->
        get_dobjparamlist_for_dobj_type(dt);
    dobjparam* dp = parlist->goto_first();
    paramnames::PAR_TYPE pt = paramnames::PAR_FIRST;
    bool confused = false;
    while(dp) {
        paramnames::PAR_TYPE dpt = dp->get_partype();
        char const* mparname = get_paramnames()->get_name(dpt);
        if (strcmp(parname, mparname) == 0) {
            if (pt != paramnames::PAR_FIRST)
                confused = true;
            pt = dpt;
        }
        dp = parlist->goto_next();
    }
    if (confused) {
        *err_msg = " there seems to be more than one parameter named ";
        *err_msg += parname;
        *err_msg += ". cannot choose. please inform wcnt developer";
        invalidate();
        delete parlist;
        return false;
    }
    if (pt == paramnames::PAR_FIRST) {
        *err_msg = "\ndata object ";
        *err_msg += dobject->get_username();
        *err_msg += " does not have any parameter named ";
        *err_msg += parname;
        invalidate();
        delete parlist;
        return false;
    }
    if (!setpar::set_dobj_param(dobject, parname, pt, valstr, 0)) {
        *err_msg = "\n";
        *err_msg += setpar::err_msg;
        invalidate();
        delete parlist;
        return false;
    }
    delete parlist;
    return true;
}

bool paramedit::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_STR_UNNAMED:
        set_name((char*)data);
        retv = true;
        break;
    case paramnames::PAR_STR_LIST:
        set_parstr((char*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* paramedit::get_param(paramnames::PAR_TYPE dt)
{
    const void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_STR_UNNAMED:
        retv = get_name();
        break;
    case paramnames::PAR_STR_LIST:
        retv = get_parstr();
        break;
    default:
        retv = 0;
    }
    return retv;
}

void paramedit::create_params()
{
    if (done_params == true)
        return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_EDIT_PARAM, paramnames::PAR_STR_UNNAMED);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_EDIT_PARAM, paramnames::PAR_STR_LIST);
    done_params = true;
}

bool paramedit::done_params = false;

#endif
