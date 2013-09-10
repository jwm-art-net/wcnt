#ifndef MODINPUTSLIST_H
#include "../include/modinputlist.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"
#include "../include/listwork.h"

modinputlist::modinputlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
{
}

modinput* modinputlist::add_input(synthmod* sm, inputnames::IN_TYPE ot)
{
    modinput* mi = new modinput(sm, ot);
    llitem* tmp = add_at_tail(mi);
    if (!tmp) {
        delete mi;
        return 0;
    }
    return mi;
}

void modinputlist::delete_module_inputs(synthmod* sm)
{
    llitem* initem = find_in_data(sneak_first(), sm);
    while(initem){
        delete initem->get_data();
        llitem* tmp = initem;
        initem = find_in_data(initem->get_next(), sm);
        unlink_item(tmp);
        delete tmp;
    }
}

/*
modinputlist* modinputlist::get_list_of_category(iocat::IOCAT oc)
{
    if (is_empty() || oc <= iocat::FIRST || oc >= iocat::LAST)
        return 0;
    modinput* input = goto_first();
    modinputlist* incatlist = new modinputlist(PRESERVE_DATA);
    const inputnames& innames = jwm.get_inputnames();
    inputnames::IN_TYPE ot;
    while(input) {
        ot = input->get_inputtype();
        if (innames.get_category(ot) == oc)
            incatlist->add_input(input->get_synthmodule(), ot);
        input = goto_next();
    }
    return incatlist;
}
*/

/*
modinputlist*
modinputlist::get_list_of_category_orderpref(iocat::IOCAT in_cat,
        synthmodnames::SYNTH_MOD_TYPE* sm_prefs,
        inputnames::IN_TYPE* in_prefs,
        inputnames::IN_TYPE* not_in_prefs )
{
    if (is_empty())
        return 0;
    if (in_cat <= iocat::FIRST || in_cat >= iocat::LAST)
        return 0;
    modinputlist* pot_ins = get_list_of_category(in_cat);
    if (pot_ins == 0)
        return 0;
    modinputlist*
        sorted_ins = new modinputlist(PRESERVE_DATA);
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
                // invalid name input type preference
                not_in_prefs[b] = inputnames::IN_FIRST;
            }
        }
    }
    synthmod * sm;	
    // make list of inputs from pot_ins sorted with preferences.
    for(a = 0; in_prefs[a] > inputnames::IN_FIRST
            && in_prefs[a] < inputnames::IN_LAST; a++)
    {
        for(c = 0; sm_prefs[c] > synthmodnames::FIRST
                && sm_prefs[c] < synthmodnames::LAST; c++)
        {
            modinput* input = pot_ins->goto_first();
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
    short in_prefchk; // add remaining valid inputs
    short not_prefchk;
    modinput* input = pot_ins->goto_first();
    while(input != 0)
    {
        in_prefchk = 0;
        for(a = 0; a < op_count; a++)
        {
            for(c = 0; sm_prefs[c] > synthmodnames::FIRST
                    && sm_prefs[c] < synthmodnames::LAST; c++)
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
*/

#endif