#include "../include/copier.h"
#include "../include/dobjlist.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/groupnames.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


copier::copier() :
 dobj(dobjnames::DEF_COPIER),
 from_name(0), to_name(0),
 from_mod(0), to_mod(0),
 from_dobj(0), to_dobj(0)
{
    create_params();
}

copier::~copier()
{
    if (from_name) delete [] from_name;
    if (to_name) delete [] to_name;
}

bool copier::set_from_name(const char* name)
{
    if (from_name != name) {
        if (from_name) delete [] from_name;
        from_name = new char[strlen(name) + 1];
        strcpy(from_name, name); // copy to from_name.
    }
    if ((from_dobj = jwm.get_dobjlist()->get_dobj_by_name(name)))
        return true;
    if ((from_mod = jwm.get_modlist()->get_synthmod_by_name(name)))
        return true;
    return false;
}

bool copier::set_to_name(const char* name)
{
    if (to_name != name) {
        if (to_name) delete [] to_name;
        to_name = new char[strlen(name) + 1];
        strcpy(to_name, name); // copy to to_name
    }
    if (strcmp(name, jwm.get_dobjnames()->get_name(
            dobjnames::LST_EDITS)) == 0)
        return false;
    if (jwm.get_dobjlist()->get_dobj_by_name(name))
        return false;
    if (jwm.get_modlist()->get_synthmod_by_name(name))
        return false;
    return true;
}

bool copier::set_param(paramnames::PAR_TYPE pt, void* data)
{
    switch(pt)
    {
    case paramnames::COPYFROM:
        if (!set_from_name((const char* const)data))
        {
            dobjerr("Cannot copy %s, no such module or data object.",
                                                    (const char*)data);
            return false;
        }
        return true;
    case paramnames::COPYTO: {
        const char* const grpname =
            get_groupname((const char*)data);
        if (grpname) {
            delete [] grpname;
            dobjerr("Cannot copy %s to %s because the name %s contains "
                    "a '.' character which is strictly reserved for "
                    "grouped modules only.", from_name, (const char*)data,
                                                       (const char*)data);
            return false;
        }
        if (!set_to_name((const char*)data))
        {
            const char* d = jwm.get_dobjnames()->get_name(
                                                    dobjnames::LST_EDITS);
            const char* e = (strcmp(to_name, d) == 0
                                            ? "is reserved"
                                            : "already in use");
            dobjerr("Cannot copy %s to %s because the name %s is %s.",
                        from_name, (const char*)data, (const char*)data, e);
            return false;
        }
        return true;
    }
    default:
        return false;
    }
}

void const* copier::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::COPYFROM:  return from_name;
        case paramnames::COPYTO:    return to_name;
        default: return 0;
    }
}

stockerrs::ERR_TYPE copier::validate()
{
    if (!set_param(paramnames::COPYFROM, from_name))
        return stockerrs::ERR_ERROR;
    if (!set_param(paramnames::COPYTO, to_name))
        return stockerrs::ERR_ERROR;
    if (from_mod) {
        if (!(to_mod = from_mod->duplicate_module(to_name,
                                    synthmod::AUTO_CONNECT)))
        {
            dobjerr("%s", synthmod::get_error_msg());
            return stockerrs::ERR_ERROR;
        }
        if (!jwm.get_modlist()->add_module(to_mod)) {
            dobjerr("Could not add module %s copied from %s to module "
                                                       "run list. Bad.",
                        to_mod->get_username(), from_mod->get_username());
            return stockerrs::ERR_ERROR;
        }
        return stockerrs::ERR_NO_ERROR;
    }
    else if (from_dobj) {
        if (!(to_dobj = from_dobj->duplicate_dobj(to_name)))
            return stockerrs::ERR_ERROR;
        jwm.get_dobjlist()->add_dobj(to_dobj);
        return stockerrs::ERR_NO_ERROR;
    }
    return stockerrs::ERR_ERROR;
}

void copier::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::COPYFROM);
    relate_param(paramnames::COPYTO);
}



