/*
#include "../include/duplicate_list_module.h"

#include "../include/synthmodlist.h"
#include "../include/globals.h"
#include "../include/linkedlist.h"

#include <iostream>

template <typename T, typename D>
synthmod*
duplicate_list_module(T* sm, D* _data,
    const char* uname, synthmod::DUP_IO dupio)
{
    T* dup = new T(uname);
    if (dupio == synthmod::AUTO_CONNECT)
        sm->duplicate_inputs_to(dup);
    sm->duplicate_params_to(dup);
    const char* const
        module_type_name =
            module::names::get(sm->get_module_type());
    const char* const current_grp = get_groupname(sm->get_username());
    const char* const new_grp = get_groupname(uname);
    bool regroup = false;
    if (current_grp && new_grp)
        if (strcmp(current_grp, new_grp) != 0)
            regroup = true;
    synthmodlist* modlist = wcnt::jwm.get_modlist();
    if (wcnt::jwm.is_verbose())
        std::cout << "\n----------\nadding to duplicated "
            << module_type_name << " module " << uname;
    D* data = sm->goto_first();
    while (data) {
        const char* const data_grp =
            get_groupname(data->get_username());
        D* data_to_add = data;
        if (data_grp && regroup == true) {
            if (strcmp(data_grp, current_grp) == 0) {
                const char* const grpname =
                        set_groupname(new_grp, data->get_username());
                synthmod* grpdata =
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
            delete [] data_grp;
        }
        dup->add_at_tail(data_to_add);
        if (wcnt::jwm.is_verbose())
            std::cout << "\nadded " << data_to_add->get_username();
        data = sm->goto_next();
    }
    delete [] current_grp;
    delete [] new_grp;
    if (wcnt::jwm.is_verbose())
        std::cout << "\n----------";
    return dup;
}
*/
