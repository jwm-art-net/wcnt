#include "../include/edit.h"
#include "../include/connectorlist.h"
#include "../include/dobjlist.h"
#include "../include/globals.h"
#include "../include/setparam.h"
#include "../include/synthmodlist.h"
#include "../include/ui_moditem.h"
#include "../include/ui_dobjitem.h"


#include <iostream>

edit::edit() :
 dobj::base(dobj::SIN_EDIT),
 targetname(0), editstr(0)
{
}


void edit::register_ui()
{
    register_param(param::STR_UNNAMED)->add_descr("Name of module or data object");
    register_param(param::STR_LIST)->add_descr("parameter value -OR- input module output");
}


ui::dobjitem_list* edit::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}


edit::~edit()
{
    if (targetname)
        delete [] targetname;
    if (editstr)
        delete [] editstr;
}


bool edit::set_target_name(const char* n)
{
    D_BUG("name '%s'\n", n);
    char* grpmodname = 0;
    synthmod::base* sm = wcnt::jwm.get_modlist()
                    ->autogroup_or_any_get_synthmod_by_name(n, &grpmodname);
    dobj::base* dbj =wcnt::get_dobjlist()->get_dobj_by_name(n);
    if (!sm && !dbj)
        return false;
    if (targetname)
        delete [] targetname;
    if (grpmodname)
        targetname = grpmodname;
    else {
        targetname = new char[strlen(n) + 1];
        strcpy(targetname, n);
    }
    return true;
}


void edit::set_edit_str(const char* n)
{
    if (editstr)
        delete [] editstr;
    editstr = new char[strlen(n) + 1];
    strcpy(editstr, n);
    return;
}

bool edit::do_actions()
{
    synthmod::base* sm = wcnt::jwm.get_modlist()
                        ->autogroup_or_any_get_synthmod_by_name(targetname, 0);
    dobj::base* dbj =wcnt::get_dobjlist()->get_dobj_by_name(targetname);
    if (sm && dbj) { // very unlikely - should be caught before this stage
        dobjerr("A data object and module share the username %s. Cannot "
                    "edit parameters %s", targetname, editstr);
        invalidate();
        return false;
    }
    if (!sm && !dbj) {
        dobjerr("There are no data objects or modules named %s. Cannot "
                    "edit parameters.", targetname);
        invalidate();
        return false;
    }

    std::stringstream strm;
    std::string itemname;
    std::string valstr;

    ui::moditem_list* items = sm->get_ui_items();
    items->match_edit(sm);
    strm << editstr;
    strm >> itemname;
    while(!strm.eof()) {
        strm >> valstr;
        if (strm.eof()) {
            dobjerr("unexpected end of input for %s in module of type %s named %s\n",
                    itemname.c_str(), synthmod::names::get(sm->get_module_type()), targetname);
            return false;
        }
        D_BUG("item name '%s' valstr '%s'\n", itemname, valstr);
        if (setpar::is_operator(valstr.c_str())) {
            std::string n;
            strm >> n;
            if (strm.eof()) {
                dobjerr("unexpected end of input for %s in module of type %s named %s\n",
                        itemname.c_str(), synthmod::names::get(sm->get_module_type()), targetname);
                return false;
            }
            valstr += " " + n;
        }

        bool wasinput = false;
        std::string output;
        if (sm) {
            D_BUG("___module item___\n");
            const char* out = 0;
            // determine if item is parameter or input
            ui::moditem* item = items->match_item(itemname.c_str());
            D_BUG("item name: '%s'\n", itemname);

            switch(item->get_item_type()) {
                case ui::UI_PARAM: {
                    D_BUG("__param__\n");
                    ui::modparam* mp = static_cast<ui::modparam*>(item);
                    if (!mod_edit_param(sm, mp->get_param_type(), valstr.c_str()))
                        return false;
                }
                break;
                case ui::UI_INPUT: {
                    if (strcmp(valstr.c_str(), "off") != 0) {
                        strm >> output;
                        if (strm.eof()) {
                            dobjerr("output name missing for input %s in module of type %s named %s\n",
                                    itemname, synthmod::names::get(sm->get_module_type()), targetname);
                            return false;
                        }
                        out = output.c_str();
                    }
                    ui::modinput* mi = static_cast<ui::modinput*>(item);
                    if (!mod_edit_input(sm, mi->get_input_type(), valstr.c_str(), out))
                        return false;
                    wasinput = true;
                }
                break;
                default:
                    dobjerr("Error matching item %s in module of type %s named %s\n",
                            itemname.c_str(), synthmod::names::get(sm->get_module_type()), targetname);
                    return false;
            }
        }
        else {
            if (!dobj_edit_param(dbj, itemname.c_str(), valstr.c_str()))
                return false;
        }
        if (wcnt::jwm.is_verbose()) {
            if (wasinput) {
                std::cout << itemname << " " << valstr << " " << output << std::endl;
            }
            else {
                std::cout << itemname << " " << valstr << std::endl;
            }
        }
        strm >> itemname;
    }
    return true;
}


bool edit::mod_edit_param(synthmod::base* mod, param::TYPE pt, const char* valstr)
{
    if (!setpar::set_param(mod, pt, valstr, 0)) {
        dobjerr("%s", setpar::get_error_msg());
        invalidate();
        return false;
    }
    return true;
}

bool edit::mod_edit_input(synthmod::base* mod, input::TYPE it,
                          const char* outmod, const char* outname)
{
    // input it is assumed to be a valid input in mod
    // if output ot is NULL outmod assumed to be "off"
    output::TYPE ot;
    if (!outname)
        ot = output::names::get_off_type(input::names::get_off_category_for_input(it));
    else {
        ot = output::names::type(outname);
        if (ot == output::ERR_TYPE) {
            dobjerr("Unrecognised output type %s in connection for %s.",
                    outname, mod->get_username());
            invalidate();
            return false;
        }
        if (input::names::category(it) != output::names::category(ot))
        {
            dobjerr("In connection for %s output %s does not match category of input %s.",
                        mod->get_username(), outname, input::names::get(it));
            invalidate();
            return false;
        }
    }

    connectorlist* conlist = wcnt::get_connectlist();
    connector* con = conlist->get_connector_by_input(mod, it);
    conlist->delete_connector(con);
    con = new connector(mod, it, outmod, ot);
    if (wcnt::jwm.get_modlist()->get_autogroup())
        con->set_group_pending();
    con->set_edited();
    conlist->add_connector(con);

    if (wcnt::jwm.is_verbose()) {
        std::cout << "    " << input::names::get(it);
        std::cout << " <-- " << outmod;
        std::cout << " " << outname << std::endl;;
    }

    return true;
}


bool edit::dobj_edit_param(dobj::base* dob, const char* parname, const char* valstr)
{
    param::TYPE partype = param::names::type(parname);

    if (!dob->is_editable()) {
        dobjerr("Data object %s type %s is not editable",
                dob->get_username(), dobj::names::get(dob->get_object_type()));
        return 0;
    }

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


bool edit::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
    case param::STR_UNNAMED:
        if (!set_target_name((const char*)data)) {
            dobjerr("There are no data objects or modules named %s "
                    "cannot edit parameters.", (const char*)data);
            invalidate();
            return false;
        }
        return true;
    case param::STR_LIST:
        set_edit_str((const char*)data);
        return true;
    default:
        return false;
    }
}


const void* edit::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::STR_UNNAMED:   return targetname;
        case param::STR_LIST:      return editstr;
        default: return 0;
    }
}
