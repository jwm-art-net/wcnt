#include "../include/copier.h"
#include "../include/dobjlist.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/groupnames.h"
#include "../include/globals.h"
#include "../include/group.h"


copier::copier() :
 dobj::base(dobj::DEF_COPIER),
 from_name(0), to_name(0), suffix(0), count(1),
 from_mod(0), to_mod(0),
 from_dobj(0), to_dobj(0)
{
    set_flags(DO_ALLOW_IN_AUTOGROUP | DO_ALLOW_NAMELESS);
}

void copier::register_ui()
{
    register_param(param::COPYFROM);
    register_param(param::COPYTO);
    register_param(param::COUNT)->set_flags(ui::UI_OPTIONAL);
    register_param(param::SUFFIX)->set_flags(ui::UI_OPTIONAL);
}

ui::dobjitem_list* copier::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
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
    if ((from_dobj = wcnt::get_dobjlist()->get_dobj_by_name(name))) {
        if (wcnt::jwm.get_modlist()->get_autogroup()
            && from_dobj->get_object_type() == dobj::DEF_GROUP)
        {
            // FIXME: this error message needs to not be ignored!
            dobjerr("Copying a group (%s) within an autogroup section is not allowed",
                    name);
            return false;
        }
        return true;
    }
    if ((from_mod = wcnt::jwm.get_modlist()->autogroup_or_not_get_synthmod_by_name(name, 0)))
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
    if (strcmp(name, dobj::names::get(
            dobj::LST_EDITS)) == 0)
        return false;
    if (wcnt::get_dobjlist()->get_dobj_by_name(name))
        return false;
    if (wcnt::jwm.get_modlist()->autogroup_or_not_get_synthmod_by_name(name, 0))
        return false;
    return true;
}

bool copier::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::COPYFROM:
        if (!set_from_name((const char*)data))
        {
            dobjerr("Cannot copy %s, no such module or data object.",
                                                    (const char*)data);
            return false;
        }
        return true;
    case param::COPYTO: {
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
            const char* d = dobj::names::get(dobj::LST_EDITS);
            const char* e = (strcmp(to_name, d) == 0
                                            ? "is reserved"
                                            : "already in use");
            dobjerr("Cannot copy %s to %s because the name %s is %s.",
                        from_name, (const char*)data, (const char*)data, e);
            return false;
        }
        return true;
    }
    case param::COUNT:
        count = *(wcint_t*)data;
        return true;
    default:
        return false;
    }
}

const void* copier::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::COPYFROM: return from_name;
        case param::COPYTO:   return to_name;
        case param::COUNT:    return &count;
        default: return 0;
    }
}
#include <iostream>
errors::TYPE copier::validate()
{
    group* autogroup = wcnt::jwm.get_modlist()->get_autogroup();
    if (count < 1 || count > wcnt::max_copies) {
        dobjerr("count outside range of 1 ~ %d copies.", wcnt::max_copies);
        return errors::ERROR;
    }
    int start_no = 0;
    std::string copyname = to_name;
    D_BUG("count: %d\n", count);
    int ix = 0;
    int lz = 0;
    if (count > 1) { // try to determine number at end of to_name.
        int c = 0;
        int l = strlen(to_name);
        ix = l;
        do {
            // the check for potential leading zeros needs to be done before
            // updating to the next character and potentially exiting loop.
            if (c == '0')
                lz++;
            else
                lz = 0;
            ix--;
            c = to_name[ix];
            D_BUG("c:'%c' ", c);
        } while (c >= '0' && c <= '9' && ix > 0);
        if (ix + 1 < l) {
            if (lz) // leading zeros detected... change to total digits.
                lz = strlen(to_name + ix + 1);
            start_no = std::stoi(to_name + ix + 1);
            D_BUG(" start_no: %d\n", start_no);
        }
    }
    for (int i = 0; i < count; i++) {
        if (count > 1 && i > 0) {
            to_name[ix + 1] = '\0';
            std::string numstr = std::to_string(start_no + i);
            // add leading zeros if required
            numstr.insert(0, lz - std::min(std::string::size_type(lz), numstr.length()), '0');
            copyname = to_name + numstr;
        }
        if (from_mod) {
            if (!(to_mod = from_mod->duplicate_module(copyname.c_str(),
                                        synthmod::base::AUTO_CONNECT)))
            {
                dobjerr("%s", synthmod::base::get_error_msg());
                return errors::ERROR;
            }
            if (autogroup)
                autogroup->autogroup_module(to_mod);
            if (!wcnt::jwm.get_modlist()->add_module(to_mod)) {
                dobjerr("Could not add module %s copied from %s to module "
                "run list. Bad.",
                to_mod->get_username(), from_mod->get_username());
                return errors::ERROR;
            }
        }
        else if (from_dobj) {
            if (!(to_dobj = from_dobj->duplicate_dobj(copyname.c_str())))
                return errors::ERROR;
            wcnt::get_dobjlist()->add_dobj(to_dobj);
        }
    }
    return errors::NO_ERROR;
}
