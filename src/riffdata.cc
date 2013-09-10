#ifndef RIFFDATA_HH
#include "../include/riffdata.h"

riffdata::riffdata() :
 dobj(dobjnames::DEF_RIFF),
 quarter_val(0), note(0), notelist(0), editlist(0), note_item(0)
{
    notelist =
     new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    create_params();
}

riffdata::~riffdata()
{
    #ifdef NOTE_EDIT_DEBUG
    cout << "\ndestroying riff " << get_username();
    cout << "\nnotelist";
    #endif
    ll_item* tmp = notelist->goto_first();
    while(tmp)
    {
        note = (note_data*)tmp->get_data();
        #ifdef NOTE_EDIT_DEBUG
        cout << "\n\tdelete " << "note name " <<note->get_name();
        cout << " pos " << note->get_position();
        cout << " len " << note->get_length() << " vel ";
        cout << note->get_velocity();
        #endif
        delete note;
        tmp = notelist->goto_next();
    }
    delete notelist;
    if (editlist) {
        #ifdef NOTE_EDIT_DEBUG
        cout << "\ndestroying editlist";
        #endif
        tmp = editlist->goto_first();
        while(tmp)
        {
            note = (note_data*)tmp->get_data();
            #ifdef NOTE_EDIT_DEBUG
            cout << "\n\tdelete " << "note name " <<note->get_name();
            cout << " pos " << note->get_position();
            cout << " len " << note->get_length() << " vel ";
            cout << note->get_velocity();
            #endif
            delete note;
            tmp = editlist->goto_next();
        }
        delete editlist;
    }
}

note_data* riffdata::insert_and_position_note(note_data* newnote)
{
    if (!newnote)
        return 0;
    // removed checking of notename, because it's done by note_data
    // and having it here brings up wrong error message.
    if (newnote->get_note_type() == note_data::NOTE_TYPE_NORMAL) {
        note_data* tmp = lookup_data_match(
                            notelist, newnote, &note_data::get_position);
        if (tmp) delete_note(tmp);
        return ordered_insert(
            notelist, newnote, &note_data::get_position);
    }
    return edit_notes(newnote);
}

note_data* riffdata::edit_notes(note_data* editnote)
{
    note_data::NOTE_SEL_OP  note_sel_op =   editnote->get_note_sel_op();
    note_data::NOTE_SEL     note_sel =      editnote->get_note_sel();
    note_data::NOTE_OP      note_op =       editnote->get_note_op();
    note_data::NOTE_PAR     note_par =      editnote->get_note_par();
    linkedlist* shft_note_list = 0;
    if (note_par == note_data::NOTE_PAR_POS ||
        note_par == note_data::NOTE_PAR_QOPY)
    {
        shft_note_list = new linkedlist(
            linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    }
    if (!editlist) {
        editlist = new
            linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    }
    double (note_data::*note_get_funcs[5])() = {
                        &note_data::get_note_number,
                        &note_data::get_position,
                        &note_data::get_length,
                        &note_data::get_velocity,
                        &note_data::get_position }; // repeat pos is Qopy

    int lhs_ix1 = note_sel;
    int lhs_ix2 = (note_sel < 3) ? note_sel + 1 : 1;
    int rhs_ix1 = (note_par > 0) ? note_par - 1 : 0;
    int rhs_ix2 = (lhs_ix2 < 3) ? lhs_ix2 + 1 : 1;

    if (note_sel_op < note_data::NOTE_SEL_OP_IN)
        rhs_ix2 = lhs_ix2; // lhs_ix2 unused in this instance

    double (note_data::*edno_sel_func1)() = note_get_funcs[lhs_ix1];
    double (note_data::*edno_sel_func2)() = note_get_funcs[lhs_ix2];
    double (note_data::*rhs_func1)() = note_get_funcs[rhs_ix1];
    double (note_data::*rhs_func2)() = note_get_funcs[rhs_ix2];
    double n_f1, en_f1 = 0, en_f2 = 0;
    bool pronote, delnote;
    note_data* en =
        (note_data*)editlist->add_at_tail(editnote)->get_data();
    if (edno_sel_func1) en_f1 = (editnote->*edno_sel_func1)();
    if (edno_sel_func2) en_f2 = (editnote->*edno_sel_func2)();
    goto_first();
    while(note_item && note)
    {
        pronote = delnote = false;
        n_f1 = (note->*edno_sel_func1)();
        switch (note_sel_op)
        {
            case note_data::NOTE_SEL_OP_EQU:
                if (n_f1 == en_f1)
                    pronote = true;
                break;
            case note_data::NOTE_SEL_OP_LESS:
                if (n_f1 < en_f1)
                    pronote = true;
                break;
            case note_data::NOTE_SEL_OP_MORE:
                if (n_f1 > en_f1)
                    pronote = true;
                break;
            case note_data::NOTE_SEL_OP_IN:
                if (n_f1 > en_f1 && n_f1 < en_f1 + en_f2)
                    pronote = true;
                break;
            case note_data::NOTE_SEL_OP_OUT:
                if (n_f1 < en_f1 || n_f1 > en_f1 + en_f2)
                    pronote = true;
                break;
            default:
                cout << "\nIgnoring edit note " << editnote->get_name();
                cout << " it is not a valid note editing command.";
                break;
        }
        if (pronote == true) {
            double oldval = (note->*rhs_func1)();
            double newval = calc_note_param(note_op, oldval,
                                            (editnote->*rhs_func2)());
            #ifdef NOTE_EDIT_DEBUG
            cout << "\nProcessing note name " <<note->get_name();
            cout << " pos " << note->get_position();
            cout << " len " << note->get_length() << " vel ";
            cout << note->get_velocity();
            cout << "\toldval: " << oldval << "\tnewval: " << newval;
            #endif
            switch(note_par)
            {
                case note_data::NOTE_PAR_DEL: {
                    delnote = true;
                }
                break;
                case note_data::NOTE_PAR_NAME:
                {
                    const char* tmpname;
                    int itr;
                    char tr;
                    itr = (int)(newval - oldval);
                    tr = (char)itr;
                    if (itr < -60) tr = -60;
                    else if (itr > 60) tr = 60;
                    tmpname = transpose_notename(note->get_name(), tr);
                    note->set_name(tmpname);
                    #ifdef NOTE_EDIT_DEBUG
                    cout << "\t* new name ";
                    cout << tmpname << " transposed by ";
                    cout << (int)tr << " *";
                    #endif
                    if (tmpname) delete [] tmpname;
                }
                break;
                case note_data::NOTE_PAR_POS: {
                    delnote = true;
                    #ifdef NOTE_EDIT_DEBUG
                    cout << "\t* new pos  ";
                    cout << newval << " *";
                    #endif
                    if (newval >= 0) {
                        note_data* shft_note = new note_data(
                            note->get_name(), newval,
                            note->get_length(), note->get_velocity());
                        ordered_insert(shft_note_list, shft_note,
                            &note_data::get_position);
                    #ifdef NOTE_EDIT_DEBUG
                        cout<<" - from note: "<<shft_note->get_position();
                    #endif
                    }
                }
                break;
                case note_data::NOTE_PAR_LEN: {
                    #ifdef NOTE_EDIT_DEBUG
                    cout << "\t* new len ";
                    cout << newval << " *";
                    #endif
                    if (newval > 0)
                        note->set_length(newval);
                    else
                        delnote = true;
                }
                break;
                case note_data::NOTE_PAR_VEL: {
                    #ifdef NOTE_EDIT_DEBUG
                    cout << "\t* new vel ";
                    cout << newval << " *";
                    #endif
                    note->set_velocity(newval);
                }
                break;
                case note_data::NOTE_PAR_QOPY:{
                    #ifdef NOTE_EDIT_DEBUG
                    cout << "\t* new pos  ";
                    cout << newval << " *";
                    #endif
                    if (newval >= 0) {
                        note_data* shft_note = new note_data(
                            note->get_name(), newval,
                            note->get_length(), note->get_velocity());
                        ordered_insert(shft_note_list, shft_note,
                            &note_data::get_position);
                    #ifdef NOTE_EDIT_DEBUG
                        cout<<" - from note: "<<shft_note->get_position();
                    #endif
                    }
                }
                break;
                default:
                    cout << "\nIgnoring edit note ";
                    cout << editnote->get_name();
                    cout << " it is not a valid note editing command.";
                    break;
            }
            if (delnote) {
                #ifdef NOTE_EDIT_DEBUG
                cout << "\n\t*** deleting note ";
                cout << "\t name " <<note->get_name();
                cout << " pos " << note->get_position();
                cout << " len " << note->get_length() << " vel ";
                cout << note->get_velocity() << " ***";
                #endif
                note_data* tmpnote = note;
                ll_item* tmpitem = note_item;
                goto_next();
                notelist->unlink_item(tmpitem);
                delete tmpitem;
                delete tmpnote;
            }
            else goto_next();
        }
        else goto_next();
    }
    if (shft_note_list) {
        ll_item* shft_item = shft_note_list->goto_first();
        note_data* shftnote;
        while (shft_item) {
            editnote = (note_data*)ordered_insert(notelist,
                (shftnote = (note_data*)shft_item->get_data()),
                &note_data::get_position);
            shft_item = shft_note_list->goto_next();
        }
        delete shft_note_list;
    }
    return en;
}

double riffdata::calc_note_param(
    note_data::NOTE_OP op, double n1, double n2)
{
    switch(op) {
        case note_data::NOTE_OP_ADD: return n1 + n2;
        case note_data::NOTE_OP_SUB: return n1 - n2;
        case note_data::NOTE_OP_MUL: return n1 * n2;
        case note_data::NOTE_OP_DIV: return (n2 != 0) ? n1 / n2 : 0;
        default:
            return n1;
    }
}

bool riffdata::delete_note(note_data * nd)
{
    if (!nd)
        return false;
    ll_item* tmp = notelist->find_data(nd);
    if (!tmp)
        return false;
    delete notelist->unlink_item(tmp);
    delete nd;
    return true;
}

double riffdata::calc_riff_length(char beats_per_bar, char beat_value)
{
    if (!goto_last()) return 0;
    double note_off_pos = note->get_position() + note->get_length();
    short bars = 1;
    double beatlen = 0;
    double rifflen = 0;
    while (rifflen < note_off_pos){
        beatlen = (get_quartervalue() * (4.0 / (double)beat_value));
        rifflen = beats_per_bar * beatlen * bars;
        bars++;
    }
    return rifflen;
}

bool riffdata::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_QUARTER_VAL:
        set_quartervalue(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* riffdata::get_param(paramnames::PAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_QUARTER_VAL:
        retv = &quarter_val;
        break;
    default:
        retv = 0;
    }
    return retv;
}

dobj* riffdata::duplicate_dobj(const char* name)
{
    riffdata* dupriff = new riffdata;
    dupriff->set_username(name);
    dupriff->set_quartervalue(quarter_val);
    goto_first();
    #ifdef NOTE_EDIT_DEBUG
    cout << "\nduplicating riff from " << get_username();
    cout << " to " << name;
    #endif
    while (note) {
        #ifdef NOTE_EDIT_DEBUG
        cout << "\nduplicating note name " << note->get_name();
        cout << " pos " << note->get_position();
        cout << " len " << note->get_length();
        cout << " vel " << note->get_velocity() << "...";
        #endif
        note_data* dupnote = new note_data(note->get_name(),
                                    note->get_position(),
                                    note->get_length(),
                                    note->get_velocity());
        dupriff->insert_and_position_note(dupnote);
        goto_next();
    }
    return dupriff;
}

dobj const* riffdata::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_NOTE:
        if (!(retv = insert_and_position_note((note_data*)dbj)))
            *err_msg = "\ncould not add note change to ";
            *err_msg += *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to ";
        *err_msg += *get_username();
        retv = 0;
    }
    return retv;
}

stockerrs::ERR_TYPE riffdata::validate()
{
    if (!get_dparlist()->validate(
        this, paramnames::PAR_QUARTER_VAL, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_QUARTER_VAL);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void riffdata::create_params()
{
    if (riffdata::done_params == true)
        return;
    get_dparlist()->add_dobjparam(
     dobjnames::DEF_RIFF, paramnames::PAR_QUARTER_VAL);
    dobjdobjlist* dbjlist = get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_RIFF, dobjnames::LST_NOTES);
    dbjlist->add_dobjdobj(dobjnames::LST_NOTES, dobjnames::SIN_NOTE);
    riffdata::done_params = true;
}

bool riffdata::done_params = false;

#endif
