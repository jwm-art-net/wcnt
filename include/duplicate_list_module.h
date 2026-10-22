#ifndef DUPLICATE_LIST_MODULE_H
#define DUPLICATE_LIST_MODULE_H


#include "globals.h"
#include "synthmod.h"
#include "synthmodlist.h"

#include <iostream> // don't like this here but hey ho.

//////////////////////////////////////////////////////////////////////////
//
//    ONLY FOR BY (WITHIN) < typename T > =
//
//    class nnnn : public synthmod::base, public linked_list< typename D >
//
// note: this function needs to be declared as a friend so it can
//       duplicate_inputs_to & duplicate_params_to (otherwise
//       protected).
//
//////////////////////////////////////////////////////////////////////////

template <typename T, typename D>
synthmod::base*
duplicate_list_module(T* sm, D* _data,
    const char* uname, synthmod::base::DUP_IO dupio)
{
    (void)_data;
    T* dup = new T(uname);
    if (dupio == synthmod::base::AUTO_CONNECT)
        sm->duplicate_inputs_to(dup);
    sm->duplicate_params_to(dup);
    const char* const module_type_name = synthmod::names::get(sm->get_module_type());
    const char* current_grp = sm->get_name(groupnames::GROUP);
    char* new_grp = groupnames::str_get_name(uname, groupnames::GROUP); //get_groupname(uname);
    bool regroup = false;
    if (current_grp && new_grp)
        if (strcmp(current_grp, new_grp) != 0)
            regroup = true;
    synthmod::list* modlist = wcnt::jwm.get_modlist();
    if (wcnt::jwm.is_verbose())
        std::cout << "\n----------\nadding to duplicated "
            << module_type_name << " module " << uname;
    D* data = sm->goto_first();
    while (data) {
        const char* data_grp = data->get_name(groupnames::GROUP);
        D* data_to_add = data;
        if (data_grp && regroup == true) {
            if (strcmp(data_grp, current_grp) == 0) {
                char* grpname = data->make_username(groupnames::GROUP, new_grp);
                D_CONNECT("make username gives '%s'\n", grpname);
                synthmod::base* grpdata =
                            modlist->get_synthmod_by_name(grpname);
                if (grpdata)
                    data_to_add = (D*)grpdata;
                else if (wcnt::jwm.is_verbose())
                    std::cout << "\nWarning! " << module_type_name
                        << uname << " was expecting to find "
                        << grpname << " but could not."
                        "\nCheck the order of grouping in original "
                        "group definition.";
                delete [] grpname;
            }
        }
        dup->add_at_tail(data_to_add);
        if (wcnt::jwm.is_verbose())
            std::cout << "\nadded " << data_to_add->get_username();
        data = sm->goto_next();
    }
    delete [] new_grp;
    if (wcnt::jwm.is_verbose())
        std::cout << "\n----------";
    return dup;
}

#endif
