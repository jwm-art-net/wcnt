#ifndef COMBINER_H
#include "../include/combiner.h"
#include "../include/groupnames.h"

combiner::combiner(char const* uname) :
 synthmod(synthmodnames::MOD_COMBINER, combiner_count, uname),
 out_output(0), sigcount(0), meantotal(ON), wcntsiglist(0),
 wcntsig_item(0), wcntsig(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    wcntsiglist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    combiner_count++;
    create_params();
    create_moddobj();
}

combiner::~combiner()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
/*
    wcntsig is a synthmodule which would have been created
    before this module.  don't need to delete here. */
    delete wcntsiglist;
}

void const* combiner::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &out_output;
        break;
    default:
        o = 0;
    }
    return o;
}

bool combiner::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_MEAN_TOTAL:
        meantotal = *(STATUS*)data;
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* combiner::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_MEAN_TOTAL:
        return &meantotal;
    default:
        return 0;
    }
}

synthmod* combiner::duplicate_module(const char* uname, DUP_IO dupio)
{
    combiner* dup = new combiner(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    char* current_grp = get_groupname(get_username());
    char* new_grp = get_groupname(uname);
    bool regroup_wcnt_sigs = false;
    if (current_grp && new_grp) {
        if (strcmp(current_grp, new_grp) != 0) {
            regroup_wcnt_sigs = true;
        }
    }
    synthmodlist* modlist = get_modlist();
    if (get_verbose())
        cout << "\n----------\nadding to duplicated combiner " << uname;
    goto_first();
    while (wcntsig) {
        char* sig_grp = get_groupname(wcntsig->get_username());
        synthmod* sig_to_add = wcntsig;
        if (sig_grp && regroup_wcnt_sigs == true) {
            if (strcmp(sig_grp, current_grp) == 0) {
                char* grpsigname =
                        set_groupname(new_grp, wcntsig->get_username());
                synthmod* grpsig =
                            modlist->get_synthmod_by_name(grpsigname);
                if (grpsig) {
                    if (grpsig->get_module_type() ==
                                            synthmodnames::MOD_WCNTSIGNAL)
                        sig_to_add = grpsig;
                    else {
                        cout << "\nin combiner::duplicate, an attempt to";
                        cout << " fetch a wcnt_signal named "
                                                            << grpsigname;
                        cout << "resulted in finding ";
                        cout << grpsig->get_username();
                        cout << " which is not a wcnt_signal!?!?";
                    }
                }
                else if (get_verbose()) {
                    cout << "\nWarning! combiner " << uname;
                    cout << " was expecting to find " << grpsigname;
                    cout << " but could not.";
                    cout << "\nCheck the order of grouping in original";
                    cout << " group definition.";
                }
                delete [] grpsigname;
            }
            delete [] sig_grp;
        }
        dup->add_signal((wcnt_signal*)sig_to_add);
        if (get_verbose())
            cout << "\nadded " << sig_to_add->get_username();
        goto_next();
    }
    if (get_verbose())
        cout << "\n----------";
    return dup;
}

stockerrs::ERR_TYPE combiner::validate()
{
    if (!goto_first()) {
        *err_msg = " signals to combine";
        invalidate();
        return stockerrs::ERR_ATLEAST2;
    } else if (!goto_next()){
        *err_msg = " signals to combine";
        invalidate();
        return stockerrs::ERR_ATLEAST2;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* combiner::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
            *err_msg += sm->get_username();
            *err_msg += " is not a wcnt_signal";
            return 0;
        }
        if (!add_signal((wcnt_signal*)sm)) {
            *err_msg = "could not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into combiner";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        dobj::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to " + *get_username();
    return 0;
}

void combiner::init()
{
    goto_first();
    sig = wcntsig->get_output();
}

void combiner::run()
{
    total = 0;
    goto_first();
    while(wcntsig){
        total += *(wcntsig->get_output());
        goto_next();
    }
    if (meantotal == ON)
        out_output = total / sigcount;
    else
        out_output = total;
}

int combiner::combiner_count = 0;

bool combiner::done_params = false;
void combiner::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->
    add_param(synthmodnames::MOD_COMBINER, paramnames::PAR_MEAN_TOTAL);
    done_params = true;
}

bool combiner::done_moddobj = false;
void combiner::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_COMBINER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
