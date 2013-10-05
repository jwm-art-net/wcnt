
#include "../include/sanity_checks.h"

#ifdef SANITY_CHECKS
#include "../include/jwm_globals.h"
#include "../include/ladspa_loader.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

#include <iostream>
#include <cstdlib>

void module_iop_checks();

int check_mod_param_fixed_string(synthmod*, param::TYPE);

void sanity_checks()
{
// start up
    #ifdef WITH_LADSPA
    ladspa_loader* ladspaloader = new ladspa_loader;
    jwm.register_ladspaloader(ladspaloader);
    #endif
    synthmodlist* modlist = new synthmodlist(DELETE_DATA);
    jwm.register_modlist(modlist);

    std::cout << "\nPerforming sanity checks...";

// some things want to perform tests with a little randomness:
    srandom(time(0));

// the checks
    module_iop_checks();

// shutdown
    delete modlist;
    #ifdef WITH_LADSPA
    delete ladspaloader;
    #endif
}


// module_iop_checks:
// for detecting errors in the way inputs/outputs/params are handled.
// checks that each input/output/param listed for a module can be accessed
// via the module's get_... methods. It then performs tests to detect
// errors with the set_... methods, ie that return value from get_...
// matches value passed to set_...

void module_iop_checks()
{
    std::cout << "\nSanity checking module's inputs/outputs/params..."
        "\nNote: Data objects within certain types of module will not be"
        " checked.";
    synthmodlist* modlist = jwm.get_modlist();
    bool sanity = true;
    for (int i = module::ERR_TYPE + 2; i < module::LAST_TYPE; i++)
    {
        module::TYPE smt =
            (module::TYPE)i;
        const char* const modname = module::names::get(smt);
        std::cout << "\n--------------------------------------------"
            "\nChecking module type: " << modname;
        synthmod* sm = 0;
        if (!(sm = modlist->create_module(smt, modname))) {
            std::cout << "\n***** Could not create module type: "
                << modname;
            continue;
        }
        bool fail = mod_check_inputs(sm)
            | mod_check_outputs(sm)
            | mod_check_params(sm);
        if (fail) {
            std::cout << "\n\n***************** FAIL *****************\n";
            sanity = false;
        }
        modlist->delete_module(sm);
    }
    std::cout << "\n==================================================="
        "\nChecks completed, ";
    if (sanity)
        std::cout << "everything passed ok.";
    else
        std::cout << "some modules failed to pass the tests.";
    std::cout<<"\n===================================================\n";
}

bool mod_check_inputs(synthmod* sm)
{
    modinputlist::linkedlist* inlist =
        new_list_of_by(jwm.get_inputlist(), sm);

    modinput* input = inlist->goto_first();
    bool fail = false;
    if (input) {
        while(input) {
            input::TYPE it = input->get_inputtype();
            const char* const inname = input::names::get(it);
            std::cout << "\n\tChecking input: " << inname << "... ";
            iocat::TYPE ioc = input::names::category(it);
            // the 'outputs' to be used for setting the inputs...
            double          out_double  = 1.23456789;
            wcint_t         out_wcint_t   = 32154;
            samp_t          out_ulong   = 6754312;
            STATUS          out_trig    = OFF;
            STATUS          out_state   = ON;
            const char*     out_string  = "What's going on? Well....";
            const void* in_ret = 0;
            switch(ioc)
            {
                case iocat::DOUBLE:
                    in_ret = sm->set_in(it, &out_double);
                    break;
                case iocat::WCINT_T:
                    in_ret = sm->set_in(it, &out_wcint_t);
                    break;
                case iocat::SAMP_T:
                    in_ret = sm->set_in(it, &out_ulong);
                    break;
                case iocat::TRIG:
                    in_ret = sm->set_in(it, &out_trig);
                    break;
                case iocat::STATE:
                    in_ret = sm->set_in(it, &out_state);
                    break;
                case iocat::STRING:
                    in_ret = sm->set_in(it, &out_string);
                    break;
                default:
                    in_ret = 0;
            }
            bool mismatched = false;
            bool badinput = false;
            switch(ioc)
            {
                case iocat::DOUBLE:
                    if (*(double*)in_ret != out_double)
                            mismatched = true;
                        break;
                case iocat::WCINT_T:
                    if (*(wcint_t*)in_ret != out_wcint_t)
                        mismatched = true;
                    break;
                case iocat::SAMP_T:
                    if (*(samp_t*)in_ret != out_ulong)
                        mismatched = true;
                    break;
                case iocat::TRIG:
                    if (*(STATUS*)in_ret != out_trig)
                        mismatched = true;
                    break;
                case iocat::STATE:
                    if (*(STATUS*)in_ret != out_state)
                        mismatched = true;
                    break;
                case iocat::STRING:
                    if (*(char**)in_ret != out_string)
                        mismatched = true;
                    break;
                default:
                    badinput = true;
            }
            if (mismatched) {
                std::cout << "\n\t\tReturn value from set_in mismatches "
                    "value sent...";
                fail = true;
            }
            if (badinput) {
                std::cout << "\n\t\t***Very***Bad***Input***";
                fail = true;
            }
            badinput = false;
            mismatched = false;
            switch(ioc)
            {
                case iocat::DOUBLE:
                    if (*(double*)sm->get_in(it) != out_double)
                            mismatched = true;
                        break;
                case iocat::WCINT_T:
                    if (*(wcint_t*)sm->get_in(it) != out_wcint_t)
                        mismatched = true;
                    break;
                case iocat::SAMP_T:
                    if (*(samp_t*)sm->get_in(it) != out_ulong)
                        mismatched = true;
                    break;
                case iocat::TRIG:
                    if (*(STATUS*)sm->get_in(it) != out_trig)
                        mismatched = true;
                    break;
                case iocat::STATE:
                    if (*(STATUS*)sm->get_in(it) != out_state)
                        mismatched = true;
                    break;
                case iocat::STRING:
                    if (*(char**)sm->get_in(it) != out_string)
                        mismatched =true;
                    break;
                default:
                    badinput = true;
            }
            if (mismatched) {
                std::cout << "\n\t\tReturn value from get_in mismatches "
                    "value sent to set_in...";
                fail = true;
            }
            if (!fail)
                std::cout << "Ok.";
            input = inlist->goto_next();
        }
    }
    else
        std::cout << "\n\tModule has no inputs.";
    delete inlist;
    return fail;
}

bool mod_check_outputs(synthmod* sm)
{
    modoutputlist::linkedlist* outlist =
        new_list_of_by(jwm.get_outputlist(), sm);
    modoutput* output = outlist->goto_first();
    bool fail = false;
    if (output) {
        while(output) {
            output::TYPE ot = output->get_outputtype();
            const char* const outname = output::names::get(ot);
            std::cout << "\n\tChecking output: " << outname << "... ";
            if(!sm->get_out(ot)){
                std::cout << "\n\t\t**** Failed to get output ****";
                fail = true;
            }
            else
                std::cout << "Ok.";
            output = outlist->goto_next();
        }
    }
    else
        std::cout << "\n\tModule has no outputs.";
    delete outlist;
    return fail;
}

bool mod_check_params(synthmod * sm)
{
    modparamlist::linkedlist* parlist =
        new_list_of_by(jwm.get_paramlist(), sm->get_module_type());
    modparam* param = parlist->goto_first();
    bool fail = false;
    if (param) {
        while(param) {
            // the values to be used for setting the params...
            double          par_double  = -0.789;
            wcint_t           par_wcint_t   = 15241;
            samp_t   par_ulong   = 531627;
            STATUS          par_state   = ON;
            const char*     par_string  = "c#-1";
            int             par_fixstr_ix = 0;
            param::TYPE pt = param->get_paramtype();
            const char* const parname = param::names::get(pt);
            std::cout << "\n\tChecking parameter: " << parname << "... ";
            iocat::TYPE ioc = param::names::category(pt);
            bool set_ret = false;
            switch(ioc)
            {
                case iocat::DOUBLE:
                    set_ret = sm->set_param(pt, &par_double);
                    break;
                case iocat::WCINT_T:
                    set_ret = sm->set_param(pt, &par_wcint_t);
                    break;
                case iocat::SAMP_T:
                    set_ret = sm->set_param(pt, &par_ulong);
                    break;
                case iocat::STATE:
                    set_ret = sm->set_param(pt, &par_state);
                    break;
                case iocat::STRING:
                    set_ret = sm->set_param(pt, par_string);
                    break;
                case iocat::FIX_STR:
                    if ((par_fixstr_ix =
                        check_mod_param_fixed_string(sm, pt)) > -1)
                        set_ret = true;
                    break;
                default:
                    set_ret = true;
                    std::cout << "\n\t\tParameter type is too much work"
                        "for me to be bothered with...\n\t\tFor now."
                        " Sorry.";
            }
            if (!set_ret || par_fixstr_ix == -1) {
                std::cout << "\n\t\t***** Failed to set parameter *****";
                fail = true;
            }
            bool mismatched = false;
            switch(ioc)
            {
                case iocat::DOUBLE:
                    if (*(double*)sm->get_param(pt) != par_double)
                            mismatched = true;
                        break;
                case iocat::WCINT_T:
                    if (*(wcint_t*)sm->get_param(pt) != par_wcint_t)
                        mismatched = true;
                    break;
                case iocat::SAMP_T:
                    if (*(samp_t*)sm->get_param(pt) != par_ulong)
                        mismatched = true;
                    break;
                case iocat::STATE:
                    if (*(STATUS*)sm->get_param(pt) != par_state)
                        mismatched = true;
                    break;
                case iocat::STRING:
                    if (strcmp((char*)sm->get_param(pt), par_string) != 0)
                        mismatched =true;
                    break;
                case iocat::FIX_STR:
                    if (*(int*)sm->get_param(pt) != par_fixstr_ix)
                        mismatched = true;
                    break;
                default:
                    std::cout << "\n\t\t*******";
            }
            if (mismatched) {
                std::cout << "Value mismatch between set and get of "
                             "parameter %s in synthmod %s." <<
                             param::names::get(pt) <<
                             module::names::get(sm->get_module_type())
                                                            << std::endl;
                fail = true;
            }
            if (!fail)
                std::cout << "Ok.";
            param = parlist->goto_next();
        }
    }
    else
        std::cout << "\n\tModule has no parameters.";
    delete parlist;
    return fail;
}

int check_mod_param_fixed_string(synthmod* sm, param::TYPE pt)
{
    fixstrparam* fxspar = jwm.get_fxsparamlist()->get_fix_str_param(pt);
    int count = fxspar->get_substring_count();
    std::cout << "(multiple choice)... ";
    if (count < 2) {
        std::cout << "\n\t\t\tOnly single choice available."
            "\n\t\t\t*** FAIL ***";
        return -1;
    }
    int setvalue = rand() % count;
    for (int i = 0; i < count; i++){
        const char* ss = fxspar->get_substring(i);
        if (!ss) {
            std::cout << "\n\t\t\tCould not get choice: " << i
                << "\n\t\t\t*** FAIL ***";
            return -1;
        }
        delete [] ss;
    }
    if (sm->set_param(pt, &setvalue))
        return setvalue;
    return -1;
}

#endif // SANITY_CHECKS
