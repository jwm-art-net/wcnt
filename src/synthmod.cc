#include "../include/synthmod.h"

#include "../include/connectorlist.h"
#include "../include/fxsparamlist.h"
#include "../include/groupnames.h"
#include "../include/globals.h"
#include "../include/modoutputlist.h"
#include "../include/synthmodlist.h"
#include "../include/ui_moditem.h"

#include <iostream>


//------------------------------------------------------------------------
// public member methods
//------------------------------------------------------------------------

namespace synthmod
{
 base::base(TYPE mt, const char* const uname, int _flags_)
  : modtype(mt), username(0)
 {
    flags = _flags_ | SM_VALID;
    username = new char[strlen(uname) + 1];
    strcpy(username, uname);

    #ifdef DATA_STATS
    STATS_INC
    #endif
 }

 base::~base()
 {
    wcnt::get_outputlist()->delete_module_outputs(this);
    delete [] username;
    #ifdef DATA_STATS
    STATS_DEC
    #endif
 }

 void base::set_group_name(const char* const gname)
 {
    const char* oldname = username;
    username = set_groupname(gname, username);
    delete [] oldname;
 }

 const void* base::get_out(output::TYPE) const
 {
    #ifdef IO_PARANOIA
    sm_err("%s %s get output where none exist.", errors::stock::major,
                                            errors::stock::bad, username);
    #endif
    return 0;
 }

 const void* base::set_in(input::TYPE, const void*)
 {
    #ifdef IO_PARANOIA
    sm_err("%s %s set input where none exist.", errors::stock::major,
                                            errors::stock::bad, username);
    #endif
    return 0;
 }

 const void* base::get_in(input::TYPE) const
 {
    #ifdef IO_PARANOIA
    sm_err("%s %s get input where none exist.", errors::stock::major,
                                            errors::stock::bad, username);
    #endif
    return 0;
}

 bool base::set_param(param::TYPE, const void*)
 {
    #ifdef IO_PARANOIA
    sm_err("%s %s set parameter where none exist.", errors::stock::major,
                                            errors::stock::bad, username);
    #endif
    return false;
 }

 const void* base::get_param(param::TYPE) const
 {
    #ifdef IO_PARANOIA
    sm_err("%s %s get parameter where none exist.", errors::stock::major,
                                            errors::stock::bad, username);
    #endif
    return 0;
 }

 dobj::base* base::add_dobj(dobj::base*)
 {
    #ifdef IO_PARANOIA
    sm_err("%s %s module unable to contain data objects.",
                    errors::stock::major, errors::stock::bad_add, username);
    #endif
    return 0;
 }

 base* base::duplicate_module(const char* uname, DUP_IO iocon)
 {
    if (flag(SM_UNDUPLICABLE)) {
        sm_err("Duplication of module %s is forbidden due to type %s.",
                username, synthmod::names::get(modtype));
        return 0;
    }
    base* dup = wcnt::jwm.get_modlist()->create_module(modtype, uname);
    if (iocon == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    return dup;
 }

 bool base::type_registered() const
 {
    return first_done[modtype];
 }

 void base::ui_register()
 {
    if (!first_done[modtype]) {
        register_ui();
        first_done[modtype] = true;
    }
 }

//------------------------------------------------------------------------
// protected member methods
//------------------------------------------------------------------------

 void base::duplicate_inputs_to(base* to_mod) const
 {
    wcnt::get_connectlist()->duplicate_connections(this, to_mod);
 }

 void base::duplicate_params_to(base* to_mod) const
 {
    if (modtype != to_mod->get_module_type()) {
        sm_err("Failed to duplicate parameters from module %s "
               "to module %s. Module types differ.", username,
                                                 to_mod->get_username());
        to_mod->invalidate();
        return;
    }

    ui::moditem_list* items = to_mod->get_ui_items();

    if (!items)
        return;

    ui::moditem* item = items->goto_first();

    while(item) {
        if (item->get_item_type() == ui::UI_PARAM) {
            ui::modparam* mp = static_cast<ui::modparam*>(item);
            if (mp->should_duplicate()) {
                const void* data = get_param(mp->get_param_type());
                if (!data) {
                    sm_err("Failed to obtain data to duplicate parameter '%s'"
                           " from module %s to module %s.",
                                param::names::get(mp->get_param_type()),
                                username, to_mod->get_username());
                    return;
                }
                if (!to_mod->set_param(mp->get_param_type(), data)) {
                    sm_err("Failed to duplicate parameter %s from mod %s "
                               "to module %s.",
                                param::names::get(mp->get_param_type()),
                                username, to_mod->get_username());
                    to_mod->invalidate();
                    return;
                }
            }
        }
        item = items->goto_next();
    }
 }

 ui::moditem* base::register_param(param::TYPE pt)
 {
    if (!(flags & SM_VALID))
        return 0;

    ui::moditem* i = get_ui_items()->add_item(pt);

    if (!i) {
        sm_err("Failed to register param %s with module type %s.",
                param::names::get(pt),
                synthmod::names::get(modtype));
        invalidate();
    }
    return i;
 }

 ui::moditem* base::register_param(param::TYPE pt, const char* fixstr)
 {
    if (!(flags & SM_VALID))
        return 0;

    ui::moditem* i = get_ui_items()->add_item(pt, fixstr);
    if (!i) {
        sm_err("Failed to register fixed string param %s (%s) "
               "with module type %s,",  param::names::get(pt),
                                        fixstr,
                                        synthmod::names::get(modtype));
        invalidate();
    }
    return i;
 }

 ui::moditem* base::register_input(input::TYPE it)
 {
    if (!(flags & SM_VALID))
        return 0;

    ui::moditem* i = get_ui_items()->add_item(it);

    if (!i) {
        sm_err("Failed to register input %s with module %s.",
                input::names::get(it), username);
        invalidate();
    }
    return i;
 }

 ui::moditem* base::register_dobj(dobj::TYPE parent, dobj::TYPE sprog)
 {
    if (!(flags & SM_VALID))
        return 0;

    ui::moditem* i = get_ui_items()->add_item(parent, sprog);
    if (!i) {
        sm_err("Failed to register data object %s (child %s) with "
               "module type %s.", dobj::names::get(parent),
                                  dobj::names::get(sprog),
                                  synthmod::names::get(modtype));
        invalidate();
    }
    return i;
 }

 ui::moditem* base::register_comment(const char* literal)
 {
    if (!(flags & SM_VALID))
        return 0;

    ui::moditem* i = get_ui_items()->add_item(literal);
    if (!i) {
        sm_err("Failed to register comment \"%s\" with "
               "module type %s.", literal, synthmod::names::get(modtype));
        invalidate();
    }
    return i;
 }

 void base::register_output(output::TYPE t)
 {
    if (!(flags & SM_VALID))
        return;
    if (!wcnt::get_outputlist()->register_output(this, t)) {
        sm_err("Failed to register output %s with module %s.",
                output::names::get(t), username);
        invalidate();
    }
 }

 bool base::validate_param(param::TYPE pt, errors::TYPE et)
 {
    if (!get_ui_items()->data_validate(this, pt, et)) {
        sm_err("%s", param::names::get(pt));
        invalidate();
        return false;
    }
    return true;
 }


 #ifdef DEBUG
 bool base::check_inputs()
 {
    ui::moditem_list* items = get_ui_items();

    if(!items)
        return true;

    ui::moditem* item = items->goto_first();

    while(item) {
        if (*item == ui::UI_INPUT) {
            ui::modinput* mi = static_cast<ui::modinput*>(item);
            if (!get_in(mi->get_input_type())) {
                sm_err("%s Module %s does not have its %s input set.",
                                errors::stock::major, get_username(),
                                input::names::get(mi->get_input_type()));
                return false;
            }
        }
        item = items->goto_next();
    }
    return true;
 }
 #endif

 char base::err_msg[] = "";
 STATUS base::abort_status = OFF; // don't force an abort just yet.

 #ifdef DATA_STATS
 STATS_INIT(synthmod)
 #endif


 bool base::first_done[synthmod::LAST_TYPE] = { false };

}; // namespace synthmod
