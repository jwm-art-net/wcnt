#include "../include/modoutputlist.h"
#include "../include/globals.h"
#include "../include/synthmod.h"
#include "../include/listwork.h"

modoutputlist::modoutputlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
{
}

modoutput*
modoutputlist::register_output(synthmod::base* sm, int ot)
{
    modoutput* mo = new modoutput(sm, ot);
    if (!add_at_tail(mo)){
        delete mo;
        return 0;
    }
    return mo;
}

void modoutputlist::delete_module_outputs(synthmod::base* sm)
{
    llitem* outitem = find_in_data(sneak_first(), sm);
    while(outitem) {
        delete outitem->get_data();
        llitem* tmp = outitem;
        outitem = find_in_data(outitem->get_next(), sm);
        unlink_item(tmp);
        delete tmp;
    }
}

modoutputlist* modoutputlist::list_of_category(iocat::TYPE oc)
{
    if (is_empty() || oc <= iocat::ERR_TYPE || oc >= iocat::LAST_TYPE)
        return 0;
    modoutput* output = goto_first();
    modoutputlist*
        outcatlist = new modoutputlist;
    while(output != 0)
    {
        int ot = output->get_outputtype();
        if (output::names::category(ot) == oc)
            outcatlist->register_output(output->get_module(), ot);
        output = goto_next();
    }
    return outcatlist;
}

#ifdef UNUSED
bool modoutputlist::is_registered(synthmod::base* sm)
{
    if (!sm)
        return 0;
    modoutput* output = goto_first();
    while(output) {
        if (output->get_synthmodule() == sm)
            return true;
        output = goto_next();
    }
    return false;
}

modoutputlist* modoutputlist::list_of_category_orderpref(
                                    iocat::TYPE out_cat,
                                    synthmod::TYPE* sm_prefs,
                                    int* out_prefs,
                                    int* not_out_prefs);
{
    if (is_empty())
        return 0;
    if (out_cat <= iocat::FIRST || out_cat >= iocat::LAST)
        return 0;
    modoutputlist* pot_outs = list_of_category(out_cat);
    if (pot_outs == 0)
        return 0;
    modoutputlist*
        sorted_outs = new modoutputlist;
    wcint_t op_count = 0;
    wcint_t nop_count = 0;
    int* op = out_prefs;
    while (*op > output::ERR_TYPE && *op < output::OUT_LAST) {
        op_count++;
        op++;
    }
    int* nop = not_out_prefs;
    while (*nop > output::ERR_TYPE && *nop < output::OUT_LAST){
        nop_count++;
        nop++;
    }
    wcint_t a,b,c;
    for (a = 0; a < op_count; a++)
    {
        for (b = 0; b < nop_count; b++)
        {
            if (out_prefs[a] == not_out_prefs[b])
            {
                // invalid name output type preference
                out_prefs[a] = output::ERR_TYPE;
                not_out_prefs[b] = output::ERR_TYPE;
            }
        }
    }
    synthmod * sm;
    // make list of outputs from pot_outs sorted with preferences.
    for(a = 0; out_prefs[a] > output::ERR_TYPE
            && out_prefs[a] < output::OUT_LAST; a++)
    {
        for(c = 0; sm_prefs[c] > module::ERR_TYPE
                && sm_prefs[c] < module::LAST_TYPE; c++)
        {
            modoutput* output = pot_outs->goto_first();
            while(output != 0)
            {
                sm = output->get_synthmodule();
                if (out_prefs[a] == output->get_outputtype()
                        && sm_prefs[c] == sm->get_module_type())
                    sorted_outs->register_output(sm, output->get_outputtype());
                output = pot_outs->goto_next();
            }
        }
    }
    wcint_t out_prefchk; // add remaining valid outputs
    wcint_t not_prefchk;
    modoutput* output = pot_outs->goto_first();
    while(output != 0)
    {
        out_prefchk = 0;
        for(a = 0; a < op_count; a++)
        {
            for(c = 0; sm_prefs[c] > module::ERR_TYPE
                    && sm_prefs[c] < module::LAST_TYPE; c++)
            {
                sm = output->get_synthmodule();
                if (out_prefs[a] == output->get_outputtype()
                        && sm_prefs[c] == sm->get_module_type())
                    out_prefchk++;
            }
        }
        not_prefchk = 0;
        for (wcint_t b = 0; b < nop_count; b++)
        {
            if (not_out_prefs[b] == output->get_outputtype())
                not_prefchk++;
        }
        if (out_prefchk == 0 && not_prefchk == 0)
            sorted_outs->register_output(output->get_synthmodule(),
                                    output->get_outputtype());
        output = pot_outs->goto_next();
    }
    delete pot_outs;
    return sorted_outs;
}
#endif

