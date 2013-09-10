#ifndef SANITY_CHECKS_H

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

void module_iop_checks();

int check_mod_param_fixed_string(synthmod*, paramnames::PAR_TYPE);

void sanity_checks()
{
// start up
    #ifdef WITH_LADSPA
    ladspa_loader* ladspaloader = new ladspa_loader;
    jwm.register_ladspaloader(ladspaloader);
    #endif
    synthmodlist* modlist = new synthmodlist;
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
    synthmodlist & modlist = jwm.get_modlist();
    const synthmodnames & modnames = jwm.get_modnames();
    bool sanity = true;
    for (int i = synthmodnames::FIRST + 2; i < synthmodnames::LAST; i++)
    {
        synthmodnames::SYNTH_MOD_TYPE smt =
            (synthmodnames::SYNTH_MOD_TYPE)i;
        const char* const modname = modnames.get_name(smt);
        std::cout << "\n--------------------------------------------"
            "\nChecking module type: " << modname;
        synthmod* sm = 0;
        if (!(sm = modlist.create_module(smt, modname))) {
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
        modlist.delete_module(sm);
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
    modinputlist* inlist =
        jwm.get_inputlist()->getinputlist_for_module(sm);
    modinput* input = inlist->goto_first();
    bool fail = false;
    if (input) {
        while(input) {
            inputnames::IN_TYPE it = input->get_inputtype();
            const char* const inname = jwm.get_inputnames()->get_name(it);
            std::cout << "\n\tChecking input: " << inname << "... ";
            iocat::IOCAT ioc = jwm.get_inputnames()->getcategory(it);
            // the 'outputs' to be used for setting the inputs...
            double          out_double  = 1.23456789;
            short           out_short   = 32154;
            unsigned long   out_ulong   = 6754312;
            STATUS          out_trig    = OFF;
            STATUS          out_state   = ON;
            char*           out_string  = "What's going on? Well....";
            const void* in_ret = 0;
            switch(ioc)
            {
                case iocat::DOUBLE:
                    in_ret = sm->set_in(it, &out_double);
                    break;
                case iocat::SHORT:
                    in_ret = sm->set_in(it, &out_short);
                    break;
                case iocat::ULONG:
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
                case iocat::SHORT:
                    if (*(short*)in_ret != out_short)
                        mismatched = true;
                    break;
                case iocat::ULONG:
                    if (*(unsigned long*)in_ret != out_ulong)
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
                case iocat::SHORT:
                    if (*(short*)sm->get_in(it) != out_short)
                        mismatched = true;
                    break;
                case iocat::ULONG:
                    if (*(unsigned long*)sm->get_in(it) != out_ulong)
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
    modoutputlist* outlist =
        jwm.get_outputlist()->getoutputlist_for_module(sm);
    modoutput* output = outlist->goto_first();
    bool fail = false;
    if (output) {
        const outputnames & outnames = jwm.get_outputnames();
        while(output) {
            outputnames::OUT_TYPE ot = output->get_outputtype();
            const char* const outname = outnames.get_name(ot);
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
    modparamlist* parlist =
        jwm.get_paramlist()->getparamlist_for_moduletype(
            sm->get_module_type());
    modparam* param = parlist->goto_first();
    bool fail = false;
    if (param) {
        const paramnames & parnames = jwm.get_paramnames();
        while(param) {
            // the values to be used for setting the params...
            double          par_double  = -0.789;
            short           par_short   = 15241;
            unsigned long   par_ulong   = 531627;
            STATUS          par_state   = ON;
            char*           par_string  = "What's going on? Well....";
            int             par_fixstr_ix = 0;
            paramnames::PAR_TYPE pt = param->get_paramtype();
            const char* const parname = parnames.get_name(pt);
            std::cout << "\n\tChecking parameter: " << parname << "... ";
            iocat::IOCAT ioc = jwm.get_paramnames()->getcategory(pt);
            bool set_ret = false;
            switch(ioc)
            {
                case iocat::DOUBLE:
                    set_ret = sm->set_param(pt, &par_double);
                    break;
                case iocat::SHORT:
                    set_ret = sm->set_param(pt, &par_short);
                    break;
                case iocat::ULONG:
                    set_ret = sm->set_param(pt, &par_ulong);
                    break;
                case iocat::STATE:
                    set_ret = sm->set_param(pt, &par_state);
                    break;
                case iocat::STRING:
                    set_ret = sm->set_param(pt, &par_string);
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
                case iocat::SHORT:
                    if (*(short*)sm->get_param(pt) != par_short)
                        mismatched = true;
                    break;
                case iocat::ULONG:
                    if (*(unsigned long*)sm->get_param(pt) != par_ulong)
                        mismatched = true;
                    break;
                case iocat::STATE:
                    if (*(STATUS*)sm->get_param(pt) != par_state)
                        mismatched = true;
                    break;
                case iocat::STRING:
                    if (*(char**)sm->get_param(pt) != par_string)
                        mismatched =true;
                    break;
                case iocat::FIX_STR:
                    if (*(int*)sm->get_param(pt) != par_fixstr_ix)
                        mismatched = true;
                    break;
                default:
                    std::cout << "\n\t\t*******";
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

int check_mod_param_fixed_string(synthmod* sm, paramnames::PAR_TYPE pt)
{
    fixstrparam* fxspar = jwm.get_fxsparamlist()->getfix_str_param(pt);
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
#endif // SANITY_CHECKS_H
