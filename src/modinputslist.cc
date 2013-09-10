#ifndef MODINPUTSLIST_H
#include "../include/modinputslist.h"

modinputlist::modinputlist() : inlist(0), input(0)
{
    inlist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

modinputlist::~modinputlist()
{
    ll_item* tmp = inlist->goto_first();
    while(tmp != 0)
    {
        delete (modinput*)tmp->get_data();
        tmp = inlist->goto_next();
    }
    delete inlist;
}

modinput* modinputlist::add_input(synthmod* sm, inputnames::IN_TYPE ot)
{
    modinput* mi = new modinput(sm, ot);
    ll_item* tmp = inlist->add_at_tail(mi);
    if (!tmp) {
        delete mi;
        return 0;
    }
    return mi;
}

bool modinputlist::delete_module_inputs(synthmod* sm)
{
    if (inlist->is_empty())
        return false;
    input = goto_first();
    while(input)
    {
        if (input->get_synthmodule() == sm) {
            inlist->unlink_item(input_item);
            delete input;
            delete input_item;
            input = goto_first();
        }
        else
            input = goto_next();
    }
    return true;
}

bool modinputlist::delete_input(modinput* mo)
{
    if (!mo)
        return false;
    ll_item * tmp = inlist->find_data(mo);
    if (tmp)
    {
        inlist->unlink_item(tmp);
        delete mo;
        delete tmp;
        return true;
    }
    return false;
}

modinputlist* modinputlist::get_list_of_category(IOCAT oc)
{
    if (inlist->is_empty())
        return 0;
    if (oc <= CAT_FIRST || oc >= CAT_LAST)
        return 0;
    input = goto_first();
    modinputlist * incatlist = new modinputlist;
    inputnames* innames = synthmod::get_inputnames();
    inputnames::IN_TYPE ot;
    while(input != 0)
    {
        ot = input->get_inputtype();
        if (innames->get_category(ot) == oc)
            incatlist->add_input(input->get_synthmodule(), ot);
        input = goto_next();
    }
    return incatlist;
}

modinputlist* modinputlist::get_list_of_category_orderpref(IOCAT in_cat,
        synthmodnames::SYNTH_MOD_TYPE* sm_prefs,
        inputnames::IN_TYPE* in_prefs,
        inputnames::IN_TYPE* not_in_prefs )
{
    if (inlist->is_empty())
        return 0;
    if (in_cat <= CAT_FIRST || in_cat >= CAT_LAST)
        return 0;
    modinputlist* pot_ins = get_list_of_category(in_cat);
    if (pot_ins == 0)
        return 0;
    modinputlist* sorted_ins = new modinputlist;
    short op_count = 0;
    short nop_count = 0;
    inputnames::IN_TYPE* op = in_prefs;
    while (*op > inputnames::IN_FIRST && *op < inputnames::IN_LAST)
    {
        op_count++;
        op++;
    }
    inputnames::IN_TYPE* nop = not_in_prefs;
    while (*nop > inputnames::IN_FIRST && *nop < inputnames::IN_LAST)
    {
        nop_count++;
        nop++;
    }
    short a,b,c;
    for (a = 0; a < op_count; a++)
    {
        for (b = 0; b < nop_count; b++)
        {
            if (in_prefs[a] == not_in_prefs[b])
            {
                in_prefs[a] = inputnames::IN_FIRST;	
                /* invalid name input type preference */
                not_in_prefs[b] = inputnames::IN_FIRST;
            }
        }
    }
    synthmod * sm;	
    /* make list of inputs from pot_ins sorted with preferences. */
    for(a = 0; in_prefs[a] > inputnames::IN_FIRST
            && in_prefs[a] < inputnames::IN_LAST; a++)
    {
        for(c = 0; sm_prefs[c] > synthmodnames::MOD_FIRST
                && sm_prefs[c] < synthmodnames::MOD_LAST; c++)
        {
            input = pot_ins->goto_first();
            while(input != 0)
            {
                sm = input->get_synthmodule();
                if (in_prefs[a] == input->get_inputtype()
                        && sm_prefs[c] == sm->get_module_type())
                    sorted_ins->add_input(sm, input->get_inputtype());
                input = pot_ins->goto_next();
            }
        }
    }
    short in_prefchk; /* add remaining valid inputs */
    short not_prefchk;
    input = pot_ins->goto_first();
    while(input != 0)
    {
        in_prefchk = 0;
        for(a = 0; a < op_count; a++)
        {
            for(c = 0; sm_prefs[c] > synthmodnames::MOD_FIRST
                    && sm_prefs[c] < synthmodnames::MOD_LAST; c++)
            {
                sm = input->get_synthmodule();
                if (in_prefs[a] == input->get_inputtype()
                        && sm_prefs[c] == sm->get_module_type())
                    in_prefchk++;
            }
        }
        not_prefchk = 0;
        for (int b = 0; b < nop_count; b++)
        {
            if (not_in_prefs[b] == input->get_inputtype())
                not_prefchk++;
        }
        if (in_prefchk == 0 && not_prefchk == 0)
            sorted_ins->add_input(input->get_synthmodule(),
                                  input->get_inputtype());
        input = pot_ins->goto_next();
    }
    delete pot_ins;
    return sorted_ins;
}

modinputlist* modinputlist::get_inputlist_for_module(synthmod* sm)
{
    if (!sm)
        return 0;
    modinputlist* mil = new modinputlist;
    goto_first();
    while(input) {
        if (input->get_synthmodule() == sm) {
            if (!mil->add_input(input->get_synthmodule(),
                                input->get_inputtype()))
            {
                delete mil;
                return 0;
            }
        }
        goto_next();
    }
    return mil;
}

#endif
