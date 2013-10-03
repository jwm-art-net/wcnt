#include "../include/modinputlist.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"
#include "../include/listwork.h"

modinputlist::modinputlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
{
}

modinput* modinputlist::register_input(synthmod* sm, input::TYPE ot)
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
modinputlist* modinputlist::get_list_of_category(iocat::TYPE oc)
{
    if (is_empty() || oc <= iocat::FIRST || oc >= iocat::LAST)
        return 0;
    modinput* input = goto_first();
    modinputlist* incatlist = new modinputlist(PRESERVE_DATA);
    const inputnames& innames = jwm.get_inputnames();
    input::TYPE ot;
    while(input) {
        ot = input->get_inputtype();
        if (innames.get_category(ot) == oc)
            incatlist->register_input(input->get_synthmodule(), ot);
        input = goto_next();
    }
    return incatlist;
}
*/

/*
modinputlist*
modinputlist::get_list_of_category_orderpref(iocat::TYPE in_cat,
        module::TYPE* sm_prefs,
        input::TYPE* in_prefs,
        input::TYPE* not_in_prefs )
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
    wcint_t op_count = 0;
    wcint_t nop_count = 0;
    input::TYPE* op = in_prefs;
    while (*op > input::ERR_TYPE && *op < input::LAST)
    {
        op_count++;
        op++;
    }
    input::TYPE* nop = not_in_prefs;
    while (*nop > input::ERR_TYPE && *nop < input::LAST)
    {
        nop_count++;
        nop++;
    }
    wcint_t a,b,c;
    for (a = 0; a < op_count; a++)
    {
        for (b = 0; b < nop_count; b++)
        {
            if (in_prefs[a] == not_in_prefs[b])
            {
                in_prefs[a] = input::ERR_TYPE;	
                // invalid name input type preference
                not_in_prefs[b] = input::ERR_TYPE;
            }
        }
    }
    synthmod * sm;	
    // make list of inputs from pot_ins sorted with preferences.
    for(a = 0; in_prefs[a] > input::ERR_TYPE
            && in_prefs[a] < input::LAST; a++)
    {
        for(c = 0; sm_prefs[c] > module::ERR_TYPE
                && sm_prefs[c] < module::LAST_TYPE; c++)
        {
            modinput* input = pot_ins->goto_first();
            while(input != 0)
            {
                sm = input->get_synthmodule();
                if (in_prefs[a] == input->get_inputtype()
                        && sm_prefs[c] == sm->get_module_type())
                    sorted_ins->register_input(sm, input->get_inputtype());
                input = pot_ins->goto_next();
            }
        }
    }
    wcint_t in_prefchk; // add remaining valid inputs
    wcint_t not_prefchk;
    modinput* input = pot_ins->goto_first();
    while(input != 0)
    {
        in_prefchk = 0;
        for(a = 0; a < op_count; a++)
        {
            for(c = 0; sm_prefs[c] > module::ERR_TYPE
                    && sm_prefs[c] < module::LAST_TYPE; c++)
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
            sorted_ins->register_input(input->get_synthmodule(),
                                  input->get_inputtype());
        input = pot_ins->goto_next();
    }
    delete pot_ins;
    return sorted_ins;
}
*/

