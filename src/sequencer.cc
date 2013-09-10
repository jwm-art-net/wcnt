#ifndef SEQUENCER_H
#include "../include/sequencer.h"

sequencer::sequencer(char const* uname) :
 synthmod(synthmodnames::MOD_SEQUENCER, sequencer_count, uname),
 in_bar_trig(0), in_bar(0), in_pos_step_size(0), in_beats_per_bar(0),
 in_beat_value(0),
 out_note_on_trig(OFF), out_note_slide_trig(OFF),
 out_note_off_trig(OFF), out_riff_start_trig(OFF), out_riff_end_trig(OFF),
 out_start_trig(OFF), out_end_trig(OFF),
 out_notename(0), out_freq(0), out_velocity(0), out_velocity_ramp(0),
 out_transpose(0),
 riff_play_state(OFF), note_play_state(OFF),
 freq_mod1size(0), vel_response(0), 
 riffnodelist(0), cur_node(0), riff_node_ptr(0), riff_ptr(0),
 riffnodeitem(0), riff_start_bar(0), riff_pos(0),
 riff_len(0), posconv(0), velrsp_max_samps(0), velrsp_samp(0),
 vel_stpsize(0), start_pending(ON), end_pending(OFF), play_list(0),
 play_item(0), next_in_riff(0), play_note(0), next_note(0), note_ptr(0),
 next_note_on_pos(-1), play_note_off_pos(-1)
{
    get_inputlist()->add_input(this, inputnames::IN_BAR);
    get_inputlist()->add_input(this, inputnames::IN_BAR_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_POS_STEP_SIZE);
    get_inputlist()->add_input(this, inputnames::IN_BEATS_PER_BAR);
    get_inputlist()->add_input(this, inputnames::IN_BEAT_VALUE);
    get_outputlist()->add_output(this, outputnames::OUT_NOTE_ON_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOTE_SLIDE_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOTE_OFF_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOTENAME);
    get_outputlist()->add_output(this, outputnames::OUT_FREQ);
    get_outputlist()->add_output(this, outputnames::OUT_VELOCITY);
    get_outputlist()->add_output(this, outputnames::OUT_VELOCITY_RAMP);
    get_outputlist()->add_output(this, outputnames::OUT_TRANSPOSE);
    get_outputlist()->add_output(this, outputnames::OUT_RIFF_START_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_RIFF_END_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_START_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_END_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_RIFF_PLAY_STATE);
    get_outputlist()->add_output(this, outputnames::OUT_NOTE_PLAY_STATE);
    riffnodelist =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    play_list =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    sequencer_count++;
    create_params();
    create_moddobj();
}

sequencer::~sequencer()
{
    ll_item* tmp = riffnodelist->goto_first();
    while (tmp) {
        delete (riff_node*)tmp->get_data();
        tmp = riffnodelist->goto_next();
    }
    delete riffnodelist;
    tmp = play_list->goto_first();
    bool del_next = true; // next_note will get deleted even if
    while (tmp) {         // it was not added to play_list...
        note_data* tn = (note_data*)tmp->get_data();
        if (tn == next_note)
            del_next = false;
        #ifdef NOTE_EDIT_DEBUG
        cout << "\nSequencer destructor destroying note:";
        #endif
        delete tn;
        tmp = play_list->goto_next();
    }
    delete play_list;
    if (del_next) {
        #ifdef NOTE_EDIT_DEBUG
        cout << "\nSequencer destructor destroying note:";
        #endif
        delete next_note;
    }
    if (out_notename) delete [] out_notename;
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* sequencer::get_out(outputnames::OUT_TYPE ot)
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

void const* sequencer::get_in(inputnames::IN_TYPE it)
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
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_VELOCITY_RESPONSE:
        set_velocity_response_time(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* sequencer::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_VELOCITY_RESPONSE:
        return &vel_response;
    default:
        return false;
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
    if (!get_paramlist()->validate(this,
            paramnames::PAR_VELOCITY_RESPONSE,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg +=
         get_paramnames()->get_name(paramnames::PAR_VELOCITY_RESPONSE);
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
    riff_node* oldrn = 0;
    riff_node* insrn = 0;
    if (rn == NULL)
        return NULL;
    short rep = rn->get_repeat();
    short repstr = rn->get_repeat_stripe();
    do{
        oldrn = lookup_data_match(
                riffnodelist, rn, &riff_node::get_start_bar);
        if (oldrn)
            delete_riff_node(oldrn);
        insrn = ordered_insert(
                riffnodelist, rn, &riff_node::get_start_bar);
        if (!insrn) {
            return 0;
        }
        if (rep > 0)
            rn = insrn->duplicate_for_bar(
                                        insrn->get_start_bar() + repstr);
        rep--;
    }while (rep >= 0);
    return insrn;
}

riff_node* sequencer::add_riff(riffdata* rd, short barpos)
{
    if (rd == NULL)
        return NULL;
    riff_node* newriffnode = new riff_node(rd, barpos);
    if (lookup_data_match(riffnodelist,
                          newriffnode, &riff_node::get_start_bar))
    { // no more than one riff at any bar.
        delete newriffnode;
        return 0;
    }
    riff_node* tmp = 
     ordered_insert(riffnodelist,newriffnode,&riff_node::get_start_bar);
    if (tmp == NULL) {
        delete newriffnode;
        return NULL;
    }
    return tmp;
}

bool sequencer::delete_riff_node(riff_node* rn)
{
    if (!rn)
        return false;
    ll_item* tmp = riffnodelist->find_data(rn);
    if (!tmp)
        return false;
    delete rn;
    delete riffnodelist->unlink_item(tmp);
    return true;
}

void sequencer::init()
{
    velrsp_max_samps = ms_to_samples(vel_response);
    riff_node_ptr = (riff_node*)
     (riffnodeitem = riffnodelist->goto_first())->get_data();
    if (riff_node_ptr)
        riff_start_bar = riff_node_ptr->get_start_bar();
    else
// (time_map is initialised with out_bar = -1 before first run().)
        riff_start_bar = -2;
// sensible people would define it before the sequencer in a .wc file!
    freq_mod1size -= 1;
}

note_data* sequencer::posconv_note(note_data* rn)
{
    // create new note using its length to store note off position,
    // convert to time_map quarter_value, and transpose.
    const char* tr_notename =
        transpose_notename(rn->get_name(), out_transpose);
    #ifdef NOTE_EDIT_DEBUG
    cout << "\nSequencer creating new note:";
    #endif
    note_data* newnote = new note_data(
                        tr_notename,
                        rn->get_position() * posconv,
                        (rn->get_position() + rn->get_length()) * posconv,
                        rn->get_velocity());
    delete [] tr_notename;
    return newnote;
}

void sequencer::init_next_note(ll_item* riff_note_item)
{
    if (next_note != 0) {
        cout << "\nProgrammer Error! next_note is not NULL";
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
                play_item = play_list->goto_first();
                while(play_item) {
                    note_ptr = (note_data*)play_item->get_data();
                    note_ptr->set_length(
                        note_ptr->get_length() - riff_len);
                    play_item = play_list->goto_next();
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
            riff_node_ptr = (riff_node*)
             (riffnodeitem = riffnodelist->goto_next())->get_data();
            if (riff_node_ptr)
                riff_start_bar = riff_node_ptr->get_start_bar();
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
                play_item = play_list->goto_first();
                while(play_item) {
                    note_ptr = (note_data*)play_item->get_data();
                    if (riff_pos >= note_ptr->get_length()) {
                        ll_item* n = play_item->get_next();
                        play_list->unlink_item(play_item);
                        #ifdef NOTE_EDIT_DEBUG
                        cout << "\nSequencer destroying note:";
                        #endif
                        delete note_ptr;
                        delete play_item;
                        play_item = n;
                    }
                    else
                        play_item = play_item->get_next();
                }
                play_item = play_list->goto_last();
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

int sequencer::sequencer_count = 0;

bool sequencer::done_params = false;

void sequencer::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SEQUENCER, paramnames::PAR_VELOCITY_RESPONSE);
    done_params = true;
}
bool sequencer::done_moddobj = false;
void sequencer::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_SEQUENCER, dobjnames::LST_TRACK);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_TRACK, dobjnames::SIN_RIFFNODE);
    done_moddobj = true;
}

#endif
