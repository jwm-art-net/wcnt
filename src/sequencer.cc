#include "../include/sequencer.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"
#include "../include/timemap.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>

sequencer::sequencer(char const* uname) :

 synthmod(
    synthmodnames::SEQUENCER,
    uname,
    SM_UNDUPLICABLE | SM_UNGROUPABLE),

 in_bar_trig(0), in_bar(0), in_pos_step_size(0), in_beats_per_bar(0),
 in_beat_value(0),
 out_note_on_trig(OFF), out_note_slide_trig(OFF),
 out_note_off_trig(OFF), out_riff_start_trig(OFF), out_riff_end_trig(OFF),
 out_start_trig(OFF), out_end_trig(OFF),
 out_notename(0), out_freq(0), out_velocity(0), out_velocity_ramp(0),
 out_transpose(0),
 riff_play_state(OFF), note_play_state(OFF),
 start_bar(0), vel_response(0),
 cur_node(0), riff_node_ptr(0), riff_ptr(0),
 riff_start_bar(0), riff_pos(0),
 riff_len(0), posconv(0), velrsp_max_samps(0), velrsp_samp(0),
 vel_stpsize(0), start_pending(ON), end_pending(OFF), play_list(0),
 play_item(0), next_in_riff(0), play_note(0), next_note(0), note_ptr(0),
 next_note_on_pos(-1), play_note_off_pos(-1)
{
    jwm.get_inputlist()->add_input(this, inputnames::IN_BAR);
    jwm.get_inputlist()->add_input(this, inputnames::IN_BAR_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_POS_STEP_SIZE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_BEATS_PER_BAR);
    jwm.get_inputlist()->add_input(this, inputnames::IN_BEAT_VALUE);

    jwm.get_outputlist()->add_output(this, outputnames::OUT_NOTE_ON_TRIG);
    jwm.get_outputlist()->add_output(this,
                                        outputnames::OUT_NOTE_SLIDE_TRIG);
    jwm.get_outputlist()->add_output(this,
                                        outputnames::OUT_NOTE_OFF_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_NOTENAME);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_FREQ);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_VELOCITY);
    jwm.get_outputlist()->add_output(this,
                                        outputnames::OUT_VELOCITY_RAMP);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRANSPOSE);
    jwm.get_outputlist()->add_output(this,
                                        outputnames::OUT_RIFF_START_TRIG);
    jwm.get_outputlist()->add_output(this,
                                        outputnames::OUT_RIFF_END_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_START_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_END_TRIG);
    jwm.get_outputlist()->add_output(this,
                                        outputnames::OUT_RIFF_PLAY_STATE);
    jwm.get_outputlist()->add_output(this,
                                        outputnames::OUT_NOTE_PLAY_STATE);
    play_list = new linked_list<note_data>;
    create_params();
    create_moddobj();
}

sequencer::~sequencer()
{
    if (next_note
     && !play_list->find_data(play_list->sneak_first(), next_note))
        delete next_note;
    delete play_list;
    if (out_notename)
        delete [] out_notename;
}

void const* sequencer::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
    case outputnames::OUT_NOTE_ON_TRIG:
        return &out_note_on_trig;
    case outputnames::OUT_NOTE_SLIDE_TRIG:
        return &out_note_slide_trig;
    case outputnames::OUT_NOTE_OFF_TRIG:
        return &out_note_off_trig;
    case outputnames::OUT_RIFF_START_TRIG:
        return &out_riff_start_trig;
    case outputnames::OUT_RIFF_END_TRIG:
        return &out_riff_end_trig;
    case outputnames::OUT_START_TRIG:
        return &out_start_trig;
    case outputnames::OUT_END_TRIG:
        return &out_end_trig;
    case outputnames::OUT_NOTENAME:
        return &out_notename;
    case outputnames::OUT_FREQ:
        return &out_freq;
    case outputnames::OUT_VELOCITY:
        return &out_velocity;
    case outputnames::OUT_VELOCITY_RAMP:
        return &out_velocity_ramp;
    case outputnames::OUT_TRANSPOSE:
        return &out_transpose;
    case outputnames::OUT_RIFF_PLAY_STATE:
        return &riff_play_state;
    case outputnames::OUT_NOTE_PLAY_STATE:
        return &note_play_state;
    default:
        return 0;
    }
}

void const* sequencer::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_BAR_TRIG:
        return in_bar_trig = (STATUS*)o;
    case inputnames::IN_BAR:
        return in_bar = (short*)o;
    case inputnames::IN_POS_STEP_SIZE:
        return in_pos_step_size = (double*)o;
    case inputnames::IN_BEATS_PER_BAR:
        return in_beats_per_bar = (short*)o;
    case inputnames::IN_BEAT_VALUE:
        return in_beat_value = (short*)o;
    default:
        return 0;
    }
}

void const* sequencer::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
    case inputnames::IN_BAR_TRIG:
        return in_bar_trig;
    case inputnames::IN_BAR:
        return in_bar;
    case inputnames::IN_POS_STEP_SIZE:
        return in_pos_step_size;
    case inputnames::IN_BEATS_PER_BAR:
        return in_beats_per_bar;
    case inputnames::IN_BEAT_VALUE:
        return in_beat_value;
    default:
        return 0;
    }
}

bool sequencer::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::VELOCITY_RESPONSE:
            vel_response = *(double*)data;
            return true;
        case paramnames::START_BAR:
            start_bar = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* sequencer::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::VELOCITY_RESPONSE: return &vel_response;
        case paramnames::START_BAR:         return &start_bar;
        default: return 0;
    }
}

dobj* sequencer::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    switch(dbj->get_object_type())
    {
    case dobjnames::SIN_RIFFNODE:
        if (!(retv = add_riff_node((riff_node*)dbj)))
            *err_msg="\ncould not add riff node to ";
            *err_msg += get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to ";
        *err_msg += get_username();
        retv = 0;
    }
    return retv;
}

synthmod* sequencer::duplicate_module(const char* uname, DUP_IO dupio)
{
    *err_msg += "sequencer does not allow copies of it to be made.";
    return 0;
}

stockerrs::ERR_TYPE sequencer::validate()
{
    if (!jwm.get_paramlist()->validate(this,
            paramnames::VELOCITY_RESPONSE,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::VELOCITY_RESPONSE);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

// add_riff_node(riff_node*) is used exclusively by add_dobj(dobj*)
// add_dobj(dobj*) cannot be passed a riff, because it lacks the
// information of which bar to place it in.
// -- wcnt 1.25 --
// Previously, adding a riff to a bar which already contained a riff
// resulted in an error message and wcnt aborting, now, the old riff
// is replaced.

riff_node* sequencer::add_riff_node(riff_node* rn)
{
    if (!rn)
        return 0;
    llitem* oldrn_i = find_in_data_or_last_less_than(
        sneak_first(), rn, &riff_node::get_start_bar);
    short rep = rn->get_repeat();
    short repstr = rn->get_repeat_stripe();
    do{
        if (oldrn_i) {
            if (oldrn_i->get_data()->get_start_bar()
                == rn->get_start_bar())
            {
                delete oldrn_i->get_data();
                oldrn_i->set_data(rn);
            }
            else
                insert_after(oldrn_i, rn);
        }
        else
            add_at_tail(rn);
        if (rep > 0)
            rn = rn->duplicate_for_bar(rn->get_start_bar() + repstr);
        oldrn_i = find_in_data_or_last_less_than(
            oldrn_i->get_next(), rn, &riff_node::get_start_bar);
        rep--;
    }while (rep >= 0);
    return rn;
}

bool sequencer::delete_riff_node(riff_node* rn)
{
    if (!rn)
        return false;
    llitem* tmp = find_data(sneak_first(), rn);
    if (!tmp)
        return false;
    delete rn;
    delete unlink_item(tmp);
    return true;
}

void sequencer::init()
{
    velrsp_max_samps = ms_to_samples(vel_response);
    riff_node_ptr = goto_first();
// (time_map is initialised with out_bar = -1 before first run().)
// sensible people would define it before the sequencer in a .wc file!
// so here, init riff_start_bar to -2 to avoid you know what...
    riff_start_bar = -2;
    while(riff_node_ptr){
        if(riff_node_ptr->get_start_bar() == start_bar) {
            riff_start_bar = riff_node_ptr->get_start_bar() - start_bar;
            break;
        }
        else if (riff_node_ptr->get_start_bar() > start_bar) {
            riff_start_bar = riff_node_ptr->get_start_bar() - start_bar;
            break;
        }
        riff_node_ptr = goto_next();
    }
}

note_data* sequencer::posconv_note(note_data* rn)
{
    // create new note using its length to store note off position,
    // convert to time_map quarter_value, and transpose.
    const char* tr_notename =
        transpose_notename(rn->get_name(), out_transpose);
    note_data* newnote = new note_data(
                        tr_notename,
                        rn->get_position() * posconv,
                        (rn->get_position() + rn->get_length()) * posconv,
                        rn->get_velocity());
    delete [] tr_notename;
    return newnote;
}

void sequencer::init_next_note(ll_item<note_data>* riff_note_item)
{
    if (next_note != 0) {
        std::cout << "\nProgrammer Error! in sequencer "
        << get_username() << " - next_note is set!";
    }
    if (riff_note_item) {
        next_in_riff = riff_note_item;
        next_note = posconv_note((note_data*)next_in_riff->get_data());
        next_note_on_pos = next_note->get_position();
    }
    else {
        next_in_riff = 0;
        next_note = 0;
        next_note_on_pos = (unsigned long)-1;
    }
}

void sequencer::output_note(note_data* note)
{
    if (out_notename)
        delete [] out_notename;
    char* name = new char[strlen(note->get_name()) + 1];
    strcpy(name, note->get_name());
    out_notename = name;
    out_freq = note_to_freq(out_notename);
    out_velocity = note->get_velocity();
    if (velrsp_max_samps > 0) {
        vel_stpsize = (double)(out_velocity - out_velocity_ramp)
            / velrsp_max_samps;
        velrsp_samp = velrsp_max_samps;
    } 
    else
        out_velocity_ramp = out_velocity;
}

void sequencer::run()
{
    if (*in_bar_trig == ON) {
        if (*in_bar == riff_start_bar) {
            riff_play_state = ON;
            out_riff_start_trig = ON;
            // ***** adjust length of notes still ON *****
            if (note_play_state == ON) {
                play_note_off_pos -= riff_len;
                play_item = play_list->sneak_first();
                while(play_item) {
                    note_ptr = play_item->get_data();
                    note_ptr->set_length(
                        note_ptr->get_length() - riff_len);
                    play_item = play_item->get_next();
                }
            }
            cur_node = riff_node_ptr;
            riff_ptr = cur_node->get_riff_source();
            out_transpose = cur_node->get_transpose();
            posconv = (double)timemap::QUARTER_VALUE
                / riff_ptr->get_quartervalue();
            if (next_note) {        // no dangling allowed
                delete next_note;
                next_note = 0;
            }
            init_next_note(riff_ptr->sneak_first());
            riff_node_ptr = goto_next();
            if (riff_node_ptr)
                riff_start_bar = riff_node_ptr->get_start_bar()
                    - start_bar;
            else {
                end_pending = ON;
                riff_start_bar = -1;
            }
            riff_pos -= riff_len;
            riff_len = 0;
        }
        short bl = (short)(timemap::QUARTER_VALUE
         * (4.0 / (double)*in_beat_value));
        riff_len += (*in_beats_per_bar * bl);
    }
    else if (out_riff_start_trig == ON) {
        out_riff_start_trig = OFF;  // braces just to look nice
    }
    if (riff_play_state == ON) {
        if (riff_pos >= next_note_on_pos) {
            if (start_pending == ON) {
                start_pending = OFF;
                out_start_trig = ON;
            }
            if (note_play_state == OFF)
                note_play_state = out_note_on_trig = ON;
            else 
                out_note_slide_trig = ON;
            play_note = next_note;
            next_note = 0;
            output_note(play_note);
            play_list->add_at_tail(play_note);
            play_note_off_pos = play_note->get_length(); // (remember?)
            init_next_note(next_in_riff->get_next());
        }
        else {
            if (out_start_trig == ON) out_start_trig = OFF;
            if (out_note_on_trig == ON) out_note_on_trig = OFF;
            if (out_note_slide_trig == ON) out_note_slide_trig = OFF;
        }
        if (note_play_state == ON) {
            if (riff_pos >= play_note_off_pos) {
                // delete all notes in playlist that have finished...
                play_item = play_list->sneak_first();
                while(play_item) {
                    note_ptr = (note_data*)play_item->get_data();
                    if (riff_pos >= note_ptr->get_length()) {
                        ll_item<note_data>* n = play_item->get_next();
                        play_list->unlink_item(play_item);
                        #ifdef NOTE_EDIT_DEBUG
                        std::cout << "\nSequencer destroying note:";
                        #endif
                        delete note_ptr;
                        delete play_item;
                        play_item = n;
                    }
                    else
                        play_item = play_item->get_next();
                }
                play_item = play_list->sneak_last();
                if (play_item == 0) {
                    note_play_state = OFF;
                    out_note_off_trig = ON;
                    play_note_off_pos = -1;
                    if (!next_note) {
                        riff_play_state = OFF;
                        out_riff_end_trig = ON;
                        if (!riff_ptr) out_end_trig = ON;
                    }
                }
                else { // get sliding..
                    out_note_slide_trig = ON;
                    play_note = (note_data*)play_item->get_data();
                    output_note(play_note);
                    play_note_off_pos = play_note->get_length();
                }
            }
        }
        else {
            if (out_note_off_trig == ON) out_note_off_trig = OFF;
        }
        if (velrsp_samp > 0) {
            out_velocity_ramp += vel_stpsize;
            velrsp_samp--;
        }
    } // end if (riff play state == ON)
    else {
        if (out_note_off_trig == ON) out_note_off_trig = OFF;
        if (out_riff_end_trig == ON) out_riff_end_trig = OFF;
        if (end_pending == ON) {
            out_end_trig = ON;
            end_pending = OFF;
        }
        else if (out_end_trig == ON) out_end_trig = OFF;
    }
    riff_pos += *in_pos_step_size;
}



void sequencer::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::SEQUENCER, paramnames::START_BAR);
    jwm.get_paramlist()->add_param(
        synthmodnames::SEQUENCER, paramnames::VELOCITY_RESPONSE);
}

bool sequencer::done_moddobj = false;

void sequencer::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::SEQUENCER, dobjnames::LST_TRACK);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_TRACK, dobjnames::SIN_RIFFNODE);
    done_moddobj = true;
}

