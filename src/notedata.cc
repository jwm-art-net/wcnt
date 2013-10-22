#include "../include/notedata.h"
#include "../include/globals.h"
#include "../include/conversions.h"

#ifdef NOTE_EDIT_DEBUG
#include <iostream>
#endif



note_data::note_data() :
 dobj::base(dobj::SIN_NOTE),
 note_type(NOTE_TYPE_ERR),position(0), length(0), velocity(0)
{
    notename[0] = '\0';
    #ifdef DATA_STATS
    STATS_INC
    #endif
}

void note_data::register_ui()
{
    ui::dobjitem* i = 0;
    if ((i = register_param(param::NAME)))
        i->add_comment("Note name, for example, C#0 for middle C sharp.");
    register_param(param::NOTE_POS);
    register_param(param::NOTE_LEN);
    register_param(param::NOTE_VEL);
}

ui::dobjitem_list* note_data::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}


note_data::note_data(const char *name, double pos, double len, double vel)
 :
 dobj::base(dobj::SIN_NOTE),
 note_type(NOTE_TYPE_ERR), position(pos), length(len), velocity(vel)
{
    set_name(name);
    #ifdef DATA_STATS
    STATS_INC
    #endif

}

note_data::~note_data()
{
    #ifdef DATA_STATS
    STATS_DEC
    #endif
}

void note_data::set_name(const char *name)
{
    strncpy(notename, name, wcnt::note_name_len);
    notename[wcnt::note_name_len] = '\0';
    get_note_type();
}

note_data::NOTE_SEL_OP note_data::get_note_sel_op() const
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_SEL_OP_ERR;
    const char* ptr = notename + NOTE_CHR_SEL_OP;
    switch(*ptr)
    {
        case '=': return NOTE_SEL_OP_EQU;
        case '<': return NOTE_SEL_OP_LESS;
        case '>': return NOTE_SEL_OP_MORE;
        case 'I': return NOTE_SEL_OP_IN;
        case 'O': return NOTE_SEL_OP_OUT;
        default:
            return NOTE_SEL_OP_ERR;
    };
}

note_data::NOTE_SEL note_data::get_note_sel() const
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_SEL_ERR;
    const char* ptr = notename + NOTE_CHR_SEL;
    switch(*ptr)
    {
        case 'N': return NOTE_SEL_NAME;
        case 'P': return NOTE_SEL_POS;
        case 'L': return NOTE_SEL_LEN;
        case 'V': return NOTE_SEL_VEL;
        default:
            return NOTE_SEL_ERR;
    };
}

note_data::NOTE_OP note_data::get_note_op() const
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_OP_ERR;
    const char* ptr = notename + NOTE_CHR_OP;
    switch(*ptr)
    {
        case '+': return NOTE_OP_ADD;
        case '-': return NOTE_OP_SUB;
        case '*': return NOTE_OP_MUL;
        case '/': return NOTE_OP_DIV;
        default:
            return NOTE_OP_ERR;
    };
}

note_data::NOTE_PAR note_data::get_note_par() const
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_PAR_ERR;
    const char* ptr = notename + NOTE_CHR_PAR;
    switch(*ptr)
    {
        case '!': return NOTE_PAR_DEL;
        case 'N': return NOTE_PAR_NAME;
        case 'P': return NOTE_PAR_POS;
        case 'L': return NOTE_PAR_LEN;
        case 'V': return NOTE_PAR_VEL;
        case 'Q': return NOTE_PAR_QOPY;
        default:
            return NOTE_PAR_ERR;
    };
}

note_data::NOTE_TYPE note_data::get_note_type()
{/* FIXME: de-tangle & make const */
    NOTE_TYPE retv = NOTE_TYPE_ERR;
    if (check_notename(notename) == true)
        return (note_type = NOTE_TYPE_NORMAL);
    else {
        retv = note_type = NOTE_TYPE_EDIT;
        if (get_note_sel_op() == NOTE_SEL_OP_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            std::cout << "\n\tfailed on note_sel_op";
            #endif
        }
        if (get_note_sel() == NOTE_SEL_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            std::cout << "\n\tfailed on note_sel";
            #endif
        }
        if (get_note_op() == NOTE_OP_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            std::cout << "\n\tfailed on note_op";
            #endif
        }
        if (get_note_par() == NOTE_PAR_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            std::cout << "\n\tfailed on note_par";
            #endif
        }
    }
    return (note_type = retv);
}

double note_data::get_note_number() const
{
    const char* ptr = notename;
    if (note_type == NOTE_TYPE_ERR)
        return 0;
    if (note_type==NOTE_TYPE_EDIT) ptr += NOTE_CHR_NAME;
    char oct = extract_octave(ptr);
    double noteno = (60 + oct * 12 + (note_to_noteno(ptr) - 1));
    return noteno;
}

#ifdef UNUSED
const char* note_data::get_note_name() const
{
    const char* ptr = notename;
    if (note_type == NOTE_TYPE_ERR)
        return 0;
    if (note_type==NOTE_TYPE_EDIT) ptr += NOTE_CHR_NAME;
    return ptr;
}

double note_data::get_note_frequency() const
{
    const char* ptr = notename;
    if (note_type == NOTE_TYPE_ERR)
        return 0;
    if (note_type==NOTE_TYPE_EDIT) ptr += NOTE_CHR_NAME;
    return note_to_freq(ptr);
}
#endif

bool note_data::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
    case param::NAME:
        set_name((const char*)data);
        return true;
    case param::NOTE_POS:
        set_position(*(double*)data);
        return true;
    case param::NOTE_LEN:
        set_length(*(double*)data);
        return true;
    case param::NOTE_VEL:
        set_velocity(*(double*)data);
        return true;
    default:
        return false;
    }
}

const void* note_data::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::NAME:      return notename;
        case param::NOTE_POS:  return &position;
        case param::NOTE_LEN:  return &length;
        case param::NOTE_VEL:  return &velocity;
        default: return 0;
    }
}

errors::TYPE note_data::validate()
{
    if (note_type == NOTE_TYPE_ERR) {
        dobjerr("%s is problematically set with %s.",
                param::names::get(param::NAME), notename);
        invalidate();
        return errors::NOTENAME;
    } else if (note_type == NOTE_TYPE_EDIT)
        return errors::NO_ERROR;

    if (!validate_param(param::NOTE_POS, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::NOTE_LEN, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

#ifdef DATA_STATS
STATS_INIT(note_data)
#endif

