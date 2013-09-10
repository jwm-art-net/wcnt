#ifndef TRIGSWITCHER_H
#include "../include/trigswitcher.h"
#include "../include/groupnames.h"

trigswitcher::trigswitcher(char const* uname) :
 synthmod(synthmodnames::MOD_TRIGSWITCHER, trigswitcher_count, uname),
 in_trig(0), out_trig(OFF), wcnttriglist(0), wcnttrig_item(0),
 wcnttrig(0), trig(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    wcnttriglist =
        new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    trigswitcher_count++;
    create_moddobj();
}

trigswitcher::~trigswitcher()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
    delete wcnttriglist;
}

void const* trigswitcher::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        return &out_trig;
        break;
    default:
        return 0;
    }
}

void const* trigswitcher::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig = (STATUS*)o;
    default:
        return 0;
    }
}

void const* trigswitcher::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig;
    default:
        return 0;
    }
}

synthmod* trigswitcher::duplicate_module(const char* uname, DUP_IO dupio)
{
    trigswitcher* dup = new trigswitcher(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);

    char* current_grp = get_groupname(get_username());
    char* new_grp = get_groupname(uname);
    bool regroup_wcnt_trigs = false;
    if (current_grp && new_grp) {
        if (strcmp(current_grp, new_grp) != 0) {
            regroup_wcnt_trigs = true;
        }
    }
    synthmodlist* modlist = get_modlist();
    if (get_verbose())
        cout << "\n----------\nadding to duplicated trig_switcher "
                                                                << uname;
    goto_first();
    while (wcnttrig) {
        char* trig_grp = get_groupname(wcnttrig->get_username());
        synthmod* trig_to_add = wcnttrig;
        if (trig_grp && regroup_wcnt_trigs == true) {
            if (strcmp(trig_grp, current_grp) == 0) {
                char* grptrigname =
                        set_groupname(new_grp, wcnttrig->get_username());
                synthmod* grptrig =
                            modlist->get_synthmod_by_name(grptrigname);
                if (grptrig) {
                    if (grptrig->get_module_type() ==
                                        synthmodnames::MOD_WCNTTRIGGER)
                        trig_to_add = grptrig;
                    else {
                        cout << "\nin switcher::duplicate, an attempt to";
                        cout << " fetch a wcnt_trigger named "
                                                        << grptrigname;
                        cout << "resulted in finding ";
                        cout << grptrig->get_username();
                        cout << " which is not a wcnt_trigger.";
                    }
                }
                else if (get_verbose()) {
                    cout << "\nWarning! trig_switcher " << uname;
                    cout << " was expecting to find " << grptrigname;
                    cout << " but could not.";
                    cout << "\nCheck the order of grouping in original";
                    cout << " group definition.";
                }
                delete [] grptrigname;
            }
            delete [] trig_grp;
        }
        dup->add_trigger((wcnt_trigger*)trig_to_add);
        if (get_verbose())
            cout << "\nadded " << trig_to_add->get_username();
        goto_next();
    }
    return dup;
}

stockerrs::ERR_TYPE trigswitcher::validate()
{
    if (!goto_first()) {
        *err_msg = "must be at least two triggers to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        *err_msg = "must be at least two triggers to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* trigswitcher::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::MOD_WCNTTRIGGER) {
            *err_msg = "\n";
            *err_msg += sm->get_username();
            *err_msg += " is not a wcnt_trigger";
            return 0;
        }
        if (!add_trigger((wcnt_trigger*)sm)) {
            *err_msg = "\ncould not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into trigswitcher";
            return 0;
        }
        dobj::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += get_username();
    return 0;
}

void trigswitcher::init()
{
    goto_first();
    trig = wcnttrig->get_out_trig();
}

void trigswitcher::run()
{
    if (*in_trig == ON) {
        if (!(wcnttrig = goto_next()))
            wcnttrig = goto_first();
        trig = wcnttrig->get_out_trig();
    }
    out_trig = *trig;
}

int trigswitcher::trigswitcher_count = 0;

bool trigswitcher::done_moddobj = false;

void trigswitcher::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_TRIGSWITCHER, dobjnames::LST_TRIGGERS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_TRIGGERS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
