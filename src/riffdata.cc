#include "../include/riffdata.h"
#include "../include/globals.h"
#include "../include/conversions.h"

#include "../include/listwork.h"

#include <iostream> // for "ignoring note-edit" message

#ifdef NOTE_EDIT_DEBUG
using std::cout;
#endif

riffdata::riffdata() :
 dobj::base(dobj::DEF_RIFF),
 tpqn(0),
 editlist(0)
{
}

void riffdata::register_ui()
{
    register_param(param::QUARTER_VAL);
    register_dobj(dobj::LST_NOTES, dobj::SIN_NOTE);
}

ui::dobjitem_list* riffdata::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

riffdata::~riffdata()
{
    if (editlist)
        delete editlist;
}

note_data* riffdata::insert_and_position_note(note_data* newnote)
{
    #ifdef NOTE_EDIT_DEBUG
    cout << "\nriff: ";
    if (get_username())
        cout << get_username();
    cout << " insert_and_position_note"
        << "\n\tname " << newnote->get_name()
        << " pos " << newnote->get_position()
        << " len " << newnote->get_length()
        << " vel " << newnote->get_velocity();
    #endif
    if (!newnote)
        return 0;
    // removed checking of notename, because it's done by note_data
    // and having it here brings up wrong error message.
    if (newnote->get_note_type() == note_data::NOTE_TYPE_NORMAL) {
        return
            ordered_insert(this, newnote,
                &note_data::get_position)->get_data();
    }
    return edit_notes(newnote);
}

note_data* riffdata::edit_notes(note_data* editnote)
{
    if (!editnote)
        return 0;
    if (editnote->get_note_type() == note_data::NOTE_TYPE_ERR)
        return 0;
    note_data::NOTE_SEL_OP  note_sel_op =   editnote->get_note_sel_op();
    note_data::NOTE_SEL     note_sel =      editnote->get_note_sel();
    note_data::NOTE_OP      note_op =       editnote->get_note_op();
    note_data::NOTE_PAR     note_par =      editnote->get_note_par();
    linkedlist* shft_note_list = 0;
    if (note_par == note_data::NOTE_PAR_POS
     || note_par == note_data::NOTE_PAR_QOPY)
    {
        shft_note_list = new linkedlist(MULTIREF_OFF, PRESERVE_DATA);
    }
    if (!editlist) {
        editlist = new linkedlist;
    }
    double (note_data::*note_get_funcs[])() const = {
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

    double (note_data::*edno_sel_func1)() const = note_get_funcs[lhs_ix1];
    double (note_data::*edno_sel_func2)() const = note_get_funcs[lhs_ix2];
    double (note_data::*rhs_func1)() const = note_get_funcs[rhs_ix1];
    double (note_data::*rhs_func2)() const = note_get_funcs[rhs_ix2];
    double en_f1 = 0, en_f2 = 0;
    note_data* en =
        (note_data*)editlist->add_at_tail(editnote)->get_data();
    if (edno_sel_func1) en_f1 = (editnote->*edno_sel_func1)();
    if (edno_sel_func2) en_f2 = (editnote->*edno_sel_func2)();
    note_data* note = goto_first();
    while(note)
    {
        bool pronote = false;
        bool delnote = false;
        double n_f1 = (note->*edno_sel_func1)();
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
                std::cout << "\nIgnoring edit note "
                    << editnote->get_name()
                    << " it is not a valid note editing command.";
                break;
        }
        if (pronote == true) {
            double oldval = (note->*rhs_func1)();
            double newval = calc_note_param(note_op, oldval,
                                            (editnote->*rhs_func2)());
            #ifdef NOTE_EDIT_DEBUG
            cout << "\nProcessing note name " <<note->get_name()
                << " pos " << note->get_position()
                << " len " << note->get_length() << " vel "
                << note->get_velocity()
                << "\toldval: " << oldval << "\tnewval: " << newval;
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
                    cout << "\t* new name "
                        << tmpname << " transposed by "
                        << (int)tr << " *";
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
                            ordered_insert(
                                shft_note_list, shft_note,
                                &note_data::get_position);
                    #ifdef NOTE_EDIT_DEBUG
                        cout << " - from note: "
                            << shft_note->get_position();
                    #endif
                    }
                }
                break;
                case note_data::NOTE_PAR_LEN: {
                    #ifdef NOTE_EDIT_DEBUG
                    std::cout << "\t* new len ";
                    std::cout << newval << " *";
                    #endif
                    if (newval > 0)
                        note->set_length(newval);
                    else
                        delnote = true;
                }
                break;
                case note_data::NOTE_PAR_VEL: {
                    #ifdef NOTE_EDIT_DEBUG
                    std::cout << "\t* new vel ";
                    std::cout << newval << " *";
                    #endif
                    note->set_velocity(newval);
                }
                break;
                case note_data::NOTE_PAR_QOPY:{
                    #ifdef NOTE_EDIT_DEBUG
                    std::cout << "\t* new pos  ";
                    std::cout << newval << " *";
                    #endif
                    if (newval >= 0) {
                        note_data* shft_note = new note_data(
                            note->get_name(), newval,
                            note->get_length(), note->get_velocity());
                            ordered_insert(
                                shft_note_list, shft_note,
                                &note_data::get_position);
                    #ifdef NOTE_EDIT_DEBUG
                        std::cout << " - from note: ";
                        std::cout << shft_note->get_position();
                    #endif
                    }
                }
                break;
                default:
                    std::cout << "\nIgnoring edit note ";
                    std::cout << editnote->get_name();
                    std::cout << " it is not a valid note editing"
                                 " command.";
                    break;
            }
            if (delnote) {
                #ifdef NOTE_EDIT_DEBUG
                std::cout << "\n\t*** deleting note ";
                std::cout << "\t name " <<note->get_name();
                std::cout << " pos " << note->get_position();
                std::cout << " len " << note->get_length() << " vel ";
                std::cout << note->get_velocity() << " ***";
                #endif
                note_data* tmpnote = note;
                llitem* tmpitem = sneak_current();
                note = goto_next();
                delete unlink_item(tmpitem);
                delete tmpnote;
            }
            else
                note = goto_next();
        }
        else
            note = goto_next();
    }
    if (shft_note_list) {
        note_data* shftnote = shft_note_list->goto_first();
        while (shftnote) {
        #ifdef NOTE_EDIT_DEBUG
        std::cout << "\nadding edited notes\n\tname "
            << shftnote->get_name()
            << " pos " << shftnote->get_position()
            << " len " << shftnote->get_length()
            << " vel " << shftnote->get_velocity() << "...";
        #endif
            ordered_insert(this, shftnote, &note_data::get_position);
            shftnote = shft_note_list->goto_next();
        }
        delete shft_note_list;
    }
    return en;
}

double riffdata::calc_note_param(note_data::NOTE_OP op, double n1,
                                                        double n2)
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

#ifdef UNUSED
double riffdata::calc_riff_length(char beats_per_bar, char beat_value)
{
    note_data* note = goto_last();
    if (!note)
        return 0;
    double note_off_pos = note->get_position() + note->get_length();
    wcint_t bars = 1;
    double rifflen = 0;
    while (rifflen < note_off_pos){
        double beatlen = (tpqn * (4.0 / (double)beat_value));
        rifflen = beats_per_bar * beatlen * bars;
        bars++;
    }
    return rifflen;
}
#endif

bool riffdata::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::QUARTER_VAL:
            set_tpqn(*(wcint_t*)data);
            return true;
        default:
            return false;
    }
}

const void* riffdata::get_param(int pt) const
{
    switch(pt)
    {
        case param::QUARTER_VAL: return &tpqn;
        default: return 0;
    }
}

dobj::base* riffdata::duplicate_dobj(const char* name)
{
    riffdata* dupriff = new riffdata;
    dupriff->set_username(name);
    dupriff->set_tpqn(tpqn);
    note_data* note = goto_first();
    #ifdef NOTE_EDIT_DEBUG
    std::cout << "\nduplicating riff from " << get_username();
    std::cout << " to " << name;
    #endif
    while (note) {
        #ifdef NOTE_EDIT_DEBUG
        std::cout << "\nduplicating note name " << note->get_name();
        std::cout << " pos " << note->get_position();
        std::cout << " len " << note->get_length();
        std::cout << " vel " << note->get_velocity() << "...";
        #endif
        note_data* dupnote = new note_data(note->get_name(),
                                    note->get_position(),
                                    note->get_length(),
                                    note->get_velocity());
        dupriff->insert_and_position_note(dupnote);
        note = goto_next();
    }
    return dupriff;
}

const dobj::base* riffdata::add_dobj(dobj::base* dbj)
{
    dobj::base* retv = 0;
    dobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobj::SIN_NOTE:
        if (!(retv = insert_and_position_note((note_data*)dbj)))
            dobjerr("Could not add note change to %s.", get_username());
        break;
    default:
        dobjerr("%s %s to %s.", errors::stock::major,
                                errors::stock::bad_add, get_username());
        retv = 0;
    }
    return retv;
}

errors::TYPE riffdata::validate()
{
    if (!validate_param(param::QUARTER_VAL, errors::RANGE_COUNT1))
        return errors::RANGE_COUNT1;

    return errors::NO_ERROR;
}


