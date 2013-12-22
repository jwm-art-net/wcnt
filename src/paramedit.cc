#include "../include/paramedit.h"
#include "../include/dobjlist.h"
#include "../include/globals.h"
#include "../include/setparam.h"
#include "../include/synthmodlist.h"
#include "../include/ui_moditem.h"
#include "../include/ui_dobjitem.h"

#include <iostream>

paramedit::paramedit() :
 dobj::base(dobj::SIN_EDIT_PARAM),
 name(0), parstr(0)
{
}

void paramedit::register_ui()
{
    register_param(param::STR_UNNAMED)->add_descr("MODULE-OR-DATA-OBJECT");
    register_param(param::STR_LIST)
            ->add_descr("PARAMETER-NAME VALUE (...)");
}

ui::dobjitem_list* paramedit::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
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
    synthmod::base* sm = wcnt::jwm.get_modlist()->get_synthmod_by_name(n);
    dobj::base* dbj =wcnt::get_dobjlist()->get_dobj_by_name(n);
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
    synthmod::base* sm =
                wcnt::jwm.get_modlist()->get_synthmod_by_name(name);
    dobj::base* dbj =wcnt::get_dobjlist()->get_dobj_by_name(name);
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

    std::cout << "\\/\\/--{ partstr:'" << parstr << "'" << std::endl;
    strm >> parname;
    std::cout << "\\/\\/--{ parname:'" << parname << "'" << std::endl;
    do {
        strm >> valstr;

        std::cout << "\\/\\/--{ valstr:'" << valstr << "'" << std::endl;

        if (setpar::is_operator(valstr.c_str())) {
            std::string n;
            strm >> n;
            valstr += " " + n;
            std::cout << "\\/\\/--{ is operator valstr:'" << valstr << "'" << std::endl;
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
        if (wcnt::jwm.is_verbose())
            std::cout << "\n    " << parname << " " << valstr;
        strm >> parname;
    }while (!strm.eof());
    return true;
}

bool paramedit::mod_param_edit(synthmod::base* mod, const char* parname,
                                                    const char* valstr)
{

    ui::moditem_list* items = mod->get_ui_items();
    items->match_edit(mod);
    ui::moditem* item = items->match_item(parname);

    if (!item) {
        dobjerr("Module %s does not have any parameter named %s.",
                                        mod->get_username(), parname);
        invalidate();
        return false;
    }

    if (item->get_item_type() != ui::UI_PARAM) {
        dobjerr("Module %s contains item '%s' but it is not a parameter.",
                mod->get_username(), parname);
        invalidate();
        return false;
    }

    ui::modparam* mp = static_cast<ui::modparam*>(item);

    if (!setpar::set_param(mod, mp->get_param_type(), valstr, 0)) {
        dobjerr("%s", setpar::get_error_msg());
        invalidate();
        return false;
    }

    /*
    param::TYPE partype = param::names::type(parname);

    if (partype == param::ERR_TYPE) {
        dobjerr("No such parameter as '%s'.", parname);
        invalidate();
        return false;
    }

    ui::moditem_list* items = mod->get_ui_items();
    ui::moditem* item = (items != 0 ? items->goto_first() : 0);
    ui::modparam* mp = 0;

    while(item) {
        if (item->get_item_type() == ui::UI_PARAM) {
            mp = static_cast<ui::modparam*>(item);
            if (*mp == partype)
                break;
        }
        item = items->goto_next();
    }

    if (!mp) {
        dobjerr("Module %s does not have any parameter named %s.",
                                        mod->get_username(), parname);
        invalidate();
        return false;
    }

    if (!setpar::set_param(mod, partype, valstr, 0)) {
        dobjerr("%s", setpar::get_error_msg());
        invalidate();
        return false;
    }
    */

    return true;
}

bool paramedit::dobj_param_edit(dobj::base* dob, const char* parname,
                                                 const char* valstr)
{
    param::TYPE partype = param::names::type(parname);

    if (partype == param::ERR_TYPE) {
        dobjerr("No such parameter as '%s'.", parname);
        invalidate();
        return false;
    }

    ui::dobjitem_list* items = dob->get_ui_items();
    ui::dobjitem* item = (items != 0 ? items->goto_first() : 0);
    ui::dobjparam* dp = 0;

    while(item) {
        if (item->get_item_type() == ui::UI_PARAM) {
            dp = static_cast<ui::dobjparam*>(item);
            if (*dp == partype)
                break;
        }
        item = items->goto_next();
    }

    if (!dp) {
        dobjerr("Data object %s does not have any parameter named %s.",
                                        dob->get_username(), parname);
        invalidate();
        return false;
    }

    if (!setpar::set_param(dob, partype, valstr, 0)) {
        dobjerr("%s", setpar::get_error_msg());
        invalidate();
        return false;
    }

    return true;
}

bool paramedit::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
    case param::STR_UNNAMED:
        if (!set_name((const char*)data)) { std::cout << "FUCK!"<<std::endl;
            dobjerr("There are no data objects or modules named %s "
                    "cannot edit parameters.", (const char*)data);
            invalidate();
            return false;
        }
        return true;
    case param::STR_LIST:
        set_parstr((const char*)data);
        return true;
    default:
        return false;
    }
}

const void* paramedit::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::STR_UNNAMED:   return get_name();
        case param::STR_LIST:      return get_parstr();
        default: return 0;
    }
}
