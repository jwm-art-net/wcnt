#include "../include/modoutputlist.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"
#include "../include/listwork.h"

modoutputlist::modoutputlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
{
}

modoutput*
modoutputlist::register_output(synthmod* sm, outputnames::OUT_TYPE ot)
{
    modoutput* mo = new modoutput(sm, ot);
    if (!add_at_tail(mo)){
        delete mo;
        return 0;
    }
    return mo;
}

void modoutputlist::delete_module_outputs(synthmod* sm)
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

modoutputlist* modoutputlist::list_of_category(iocat::IOCAT oc)
{
    if (is_empty() || oc <= iocat::FIRST || oc >= iocat::LAST)
        return 0;
    modoutput* output = goto_first();
    modoutputlist*
        outcatlist = new modoutputlist;
    outputnames::OUT_TYPE ot;
    while(output != 0)
    {
        ot = output->get_outputtype();
        if (outputnames::get_category(ot) == oc)
            outcatlist->register_output(output->get_synthmodule(), ot);
        output = goto_next();
    }
    return outcatlist;
}

bool modoutputlist::is_registered(synthmod* mod)
{
    if (!mod)
        return 0;
    modoutput* output = goto_first();
    while(output) {
        if (output->get_synthmodule() == mod)
            return true;
        output = goto_next();
    }
    return false;
}

/*
modoutputlist* modoutputlist::list_of_category_orderpref(
 iocat::IOCAT out_cat, synthmodnames::SYNTH_MOD_TYPE* sm_prefs,
 outputnames::OUT_TYPE* out_prefs,
 outputnames::OUT_TYPE* not_out_prefs )
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
    short op_count = 0;
    short nop_count = 0;
    outputnames::OUT_TYPE* op = out_prefs;
    while (*op > outputnames::OUT_FIRST && *op < outputnames::OUT_LAST) {
        op_count++;
        op++;
    }
    outputnames::OUT_TYPE* nop = not_out_prefs;
    while (*nop > outputnames::OUT_FIRST && *nop < outputnames::OUT_LAST){
        nop_count++;
        nop++;
    }
    short a,b,c;
    for (a = 0; a < op_count; a++)
    {
        for (b = 0; b < nop_count; b++)
        {
            if (out_prefs[a] == not_out_prefs[b])
            {
                // invalid name output type preference
                out_prefs[a] = outputnames::OUT_FIRST;
                not_out_prefs[b] = outputnames::OUT_FIRST;
            }
        }
    }
    synthmod * sm;
    // make list of outputs from pot_outs sorted with preferences.
    for(a = 0; out_prefs[a] > outputnames::OUT_FIRST
            && out_prefs[a] < outputnames::OUT_LAST; a++)
    {
        for(c = 0; sm_prefs[c] > synthmodnames::FIRST
                && sm_prefs[c] < synthmodnames::LAST; c++)
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
    short out_prefchk; // add remaining valid outputs
    short not_prefchk;
    modoutput* output = pot_outs->goto_first();
    while(output != 0)
    {
        out_prefchk = 0;
        for(a = 0; a < op_count; a++)
        {
            for(c = 0; sm_prefs[c] > synthmodnames::FIRST
                    && sm_prefs[c] < synthmodnames::LAST; c++)
            {
                sm = output->get_synthmodule();
                if (out_prefs[a] == output->get_outputtype()
                        && sm_prefs[c] == sm->get_module_type())
                    out_prefchk++;
            }
        }
        not_prefchk = 0;
        for (short b = 0; b < nop_count; b++)
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
*/

