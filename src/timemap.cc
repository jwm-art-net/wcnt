#ifndef TIMEMAP_H
#include "../include/timemap.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

timemap::timemap(char const* uname) :
 synthmod(synthmodnames::TIMEMAP, uname),
 out_bar(0), out_pos_in_bar(0), out_pos_step_size(0), out_bpm(0.0),
 out_sample_total(0), out_sample_in_bar(0), bpm_item(0), bpm_map(0),
 currentbpm(0), targetbpm(0), bpmsampletot(0), bpmchangesamp(0),
 bpmchange_pos(0), bpmrampsize(0), bpmchange_ratio(0), targbpm(0),
 pos_in_bar(0), bpmchange_notelen(0), bpmchangebar(0)
{
    jwm.get_outputlist().add_output(this,outputnames::OUT_BPM);
    jwm.get_outputlist().add_output(this,outputnames::OUT_BAR);
    jwm.get_outputlist().add_output(this,outputnames::OUT_BAR_TRIG);
    jwm.get_outputlist().add_output(this,outputnames::OUT_POS_IN_BAR);
    jwm.get_outputlist().add_output(this,outputnames::OUT_POS_STEP_SIZE);
    jwm.get_outputlist().add_output(this,outputnames::OUT_SAMPLE_TOTAL);
    jwm.get_outputlist().add_output(this,outputnames::OUT_SAMPLE_IN_BAR);
    jwm.get_outputlist().add_output(this,outputnames::OUT_BEATS_PER_BAR);
    jwm.get_outputlist().add_output(this,outputnames::OUT_BEAT_VALUE);
    jwm.get_outputlist().add_output(this,
                                    outputnames::OUT_BPM_CHANGE_TRIG);
    jwm.get_outputlist().add_output(this,
                                    outputnames::OUT_METER_CHANGE_TRIG);
    jwm.get_outputlist().add_output(this,
                                    outputnames::OUT_BPM_CHANGE_STATE);
    bpm_map =
        new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    add_bpm_change(0, 0);
    meter_map =
        new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    add_meter_change(0, jwm.beats_per_measure(), jwm.beat_value());
    create_moddobj();
}

timemap::~timemap()
{
    jwm.get_outputlist().delete_module_outputs(this);
    goto_first_bpm();
    while(currentbpm) {
        delete currentbpm;
        goto_next_bpm();
    }
    delete bpm_map;
    goto_first_meter();
    while(currentmeter) {
        delete currentmeter;
        goto_next_meter();
    }
    delete meter_map;
}

bpmchange* timemap::add_bpm_change(short atbar, double bpm)
{
    if (!goto_first_bpm()) { // create very first bpm change
        currentbpm = new bpmchange(atbar, bpm);
        bpm_item = bpm_map->add_at_head(currentbpm);
        return currentbpm;
    }
    if (atbar == 0) {// just change it, not create.
        currentbpm->set_bpm(bpm);
        return currentbpm;
    }
    while(currentbpm) {
        // first must always be at bar zero=done by constructor.
        bpmchange* bc = (bpmchange*)bpm_map->sneak_next()->get_data();
        if (currentbpm->get_bar() == atbar) {
            if (!bc) { // currentbpm is last in list - safe to add
                currentbpm = new bpmchange(atbar, bpm);
                bpm_item = bpm_map->add_at_tail(currentbpm);
                return currentbpm;
            }
            if (bc->get_bar() == atbar) return 0;
            if (bc->get_bar() > atbar){
                currentbpm  = new bpmchange(atbar, bpm);
                bpm_item = bpm_map->insert_after(bpm_item, currentbpm);
                return currentbpm;
            }
        }
        if (bc) {
            if (bc->get_bar() > atbar) { // insert after current
                currentbpm = new bpmchange(atbar, bpm);
                bpm_item = bpm_map->insert_after(bpm_item, currentbpm);
                return currentbpm;
            }
        } else {
            currentbpm = new bpmchange(atbar, bpm);
            bpm_item = bpm_map->add_at_tail(currentbpm);
            return currentbpm;
        }
        goto_next_bpm();
    } // while(currentbpm)
    return 0;  // should not ever reach this place
}

bpmchange* timemap::add_bpm_change(bpmchange * bch)
{
    goto_first_bpm();
    if (bch->get_bar() == 0) {// just replace, not insert
        delete currentbpm;// delete meterchange object
        bpm_item->set_data(bch);
        return (currentbpm = bch);
    }
    while(currentbpm){
        // first must always be at bar zero,done by constructor.
        bpmchange* bc = (bpmchange*)bpm_map->sneak_next()->get_data();
        if (currentbpm->get_bar() == bch->get_bar()) {
            if (!bc) { // currentbpm is last in list - safe to add
                bpm_item = bpm_map->add_at_tail(bch);
                return (currentbpm = bch);
            }
            if (bc->get_bar() == bch->get_bar()) return 0;
            if (bc->get_bar() > bch->get_bar()){
                bpm_item = bpm_map->insert_after(bpm_item, bch);
                return (currentbpm = bch);
            }
        }
        if (bc) {
            if (bc->get_bar() > bch->get_bar()) { // insert after current
                bpm_item = bpm_map->insert_after(bpm_item, bch);
                return (currentbpm = bch);
            }
        } else {
            bpm_item = bpm_map->add_at_tail(bch);
            return (currentbpm = bch);
        }
        goto_next_bpm();
    } // while(currentbpm)
    return 0;  // should not ever get here..
}

meterchange* timemap::add_meter_change(short atbar, char btpb, char btval)
{
    if (!goto_first_meter()) { // create very first meter
        currentmeter = new meterchange(atbar, btpb, btval);
        meter_item = meter_map->add_at_head(currentmeter);
        return currentmeter;
    }
    if (atbar == 0) {// just change it, not create.
        currentmeter->set_beatsperbar(btpb);
        currentmeter->set_beatvalue(btval);
        return currentmeter;
    }
    while(currentmeter){
        // first must always be at bar zero=done by constructor.
        meterchange* mc = (meterchange*)
                          meter_map->sneak_next()->get_data();
        if (currentmeter->get_bar() == atbar) { // change not create
            currentmeter->set_beatsperbar(btpb);
            currentmeter->set_beatvalue(btval);
            return currentmeter;
        }
        if (mc) {
            if (mc->get_bar() > atbar) { // insert after current
                currentmeter = new meterchange(atbar, btpb, btval);
                meter_item =
                    meter_map->insert_after(meter_item, currentmeter);
                return currentmeter;
            }
        } else {
            currentmeter = new meterchange(atbar, btpb, btval);
            meter_item = meter_map->add_at_tail(currentmeter);
            return currentmeter;
        }
        goto_next_meter();
    } // while(currentmeter)
    return 0;
}

meterchange* timemap::add_meter_change(meterchange* mch)
{
    goto_first_meter();
    // don't forget the list will *never* be empty
    if (mch->get_bar() == 0) {
        delete currentmeter;
        meter_item->set_data(mch);
        return (currentmeter = mch);
    }
    while(currentmeter){
        if (currentmeter->get_bar() == mch->get_bar()) {
            delete currentmeter;
            meter_item->set_data(mch);
            return (currentmeter = mch);
        }
        meterchange* mc = (meterchange*)
                          meter_map->sneak_next()->get_data();
        if (mc) {
            if (mc->get_bar() > mch->get_bar()) {
                meter_item = meter_map->insert_after(meter_item, mch);
                return (currentmeter = mch);
            }
        } else {
            meter_item = meter_map->add_at_tail(mch);
            return (currentmeter = mch);
        }
        goto_next_meter();
    }
    return 0;
}

void timemap::init()
{
    goto_first_bpm();
    bpmchangebar = currentbpm->get_bar(); // = 0;
    targetbpm = currentbpm;
    goto_first_meter();
    meterchangebar = currentmeter->get_bar();
    out_beats_per_bar = currentmeter->get_beatsperbar();
    out_beat_value = currentmeter->get_beatvalue();
    beatlength = (short)(QUARTER_VALUE * (4.0 / (float)out_beat_value));
    barlength = out_beats_per_bar * beatlength;
    pos_in_bar = barlength; // trig first bar - not favorite sollution
    out_bar = -1;           // ...it just gets worse!
    p_bpm = jwm.bpm();
}

void timemap::run()
{
    if (pos_in_bar >= barlength) {
        out_bar++;
        pos_in_bar -= barlength;
        out_bar_trig = ON;
    }
    else if (out_bar_trig == ON) out_bar_trig = OFF;
    // do meter changes before bpm changes
    if (out_bar == meterchangebar) {
        if (currentmeter) {
            out_beats_per_bar = currentmeter->get_beatsperbar();
            out_beat_value = currentmeter->get_beatvalue();
            beatlength = (short)(QUARTER_VALUE *
                                 (4.0 / (double)out_beat_value));
            barlength = out_beats_per_bar * beatlength;
            out_meter_change_trig = ON;
            goto_next_meter();
            if (!currentmeter) meterchangebar = -1;
            else meterchangebar = currentmeter->get_bar();
        }
    }
    else if (out_meter_change_trig == ON) out_meter_change_trig = OFF;
    if (out_bar == bpmchangebar) {
        currentbpm = targetbpm;
        out_bpm = p_bpm + currentbpm->get_bpm();
        p_bpm = out_bpm;
        bpm_item = bpm_item->get_next();
        targetbpm = (bpmchange*)bpm_item->get_data();
        out_bpm_change_trig = ON;
        if (!targetbpm) {
            bpmchangebar = -1;
            bpmrampsize = 0;
        }
        else {
            bpmchangebar = targetbpm->get_bar();
            if (bpmchangebar == out_bar) {
                //immediate change -- not ramped.
                currentbpm = targetbpm;
                out_bpm += currentbpm->get_bpm();
                p_bpm = out_bpm; // make it so.
                bpm_item = bpm_item->get_next();
                targetbpm = (bpmchange*)bpm_item->get_data();
                out_bpm_change_state = ON;
            }
            if (!targetbpm) {
                bpmchangebar = -1; // prevent unwanted repetition
                bpmrampsize = 0;
            }
            else {
                bpmchangebar = targetbpm->get_bar();
                bpmchange_notelen = (unsigned long)
                    ((bpmchangebar - currentbpm->get_bar()) * barlength);
                targbpm = p_bpm + targetbpm->get_bpm();
                // these will change during bpm ramp
                bpmchange_pos = 0;
                bpmsampletot = notelen_to_samples(bpmchange_notelen);
                bpmrampsize = (targbpm - out_bpm) / (double) bpmsampletot;
                out_bpm_change_state = ON;
            }
        }
        out_pos_step_size = barlength / (jwm.samplerate() *
         (60.0 / out_bpm) * out_beats_per_bar);
    }
    else {
        if (out_bpm_change_trig == ON) out_bpm_change_trig = OFF;
        if (bpmrampsize != 0) {
            if (out_meter_change_trig == ON) {// there's always one!
                bpmchange_notelen = (unsigned long)
                    ((bpmchangebar - out_bar) * barlength);
                bpmchange_pos = 0;
                bpmsampletot = notelen_to_samples(bpmchange_notelen);
                bpmrampsize = (targbpm - out_bpm) / (double) bpmsampletot;
                out_pos_step_size = barlength / (jwm.samplerate() *
                 (60.0 / out_bpm) * out_beats_per_bar);
            }
            out_bpm += bpmrampsize;
            bpmchange_ratio = (double) bpmchange_pos / bpmchange_notelen;
            bpmsampletot
             = (unsigned long)(jwm.samplerate() * (60 / out_bpm)
             * ((double) (bpmchange_notelen - bpmchange_pos) 
             / beatlength));
            bpmrampsize = (targbpm - out_bpm) / (double) bpmsampletot;
            out_pos_step_size
             = barlength / (jwm.samplerate() * (60 / out_bpm) 
             * out_beats_per_bar);
            bpmchange_pos += out_pos_step_size;
        } else out_bpm_change_state = OFF;
    }
    out_sample_total++;
    out_sample_in_bar++;
    out_pos_in_bar = (unsigned long)pos_in_bar;
    pos_in_bar += out_pos_step_size;
}

unsigned long timemap::notelen_to_samples(short nl)
{
    return (unsigned long)(jwm.samplerate()
     * ((double) 60 / out_bpm) * ((double)nl / beatlength));
}

double timemap::notelen_to_ms(short nl)
{
    return ((double)60 / out_bpm) * ((double)nl / beatlength) * 1000;
}

double timemap::notelen_to_frequency(short nl)
{
    return 1000 / (((double)60 / out_bpm) *
                   ((double)nl / beatlength) * 1000);
}

unsigned long timemap::ms_to_samples(double ms)
{
    return (unsigned long)(jwm.samplerate() * (ms / 1000));
}

void const* timemap::get_out(outputnames::OUT_TYPE ot) const
{
    switch (ot) {
    case outputnames::OUT_BPM:
        return &out_bpm;
    case outputnames::OUT_BAR:
        return &out_bar;
    case outputnames::OUT_BAR_TRIG:
        return &out_bar_trig;
    case outputnames::OUT_POS_IN_BAR:
        return &out_pos_in_bar;
    case outputnames::OUT_POS_STEP_SIZE:
        return &out_pos_step_size;
    case outputnames::OUT_SAMPLE_TOTAL:
        return &out_sample_total;
    case outputnames::OUT_SAMPLE_IN_BAR:
        return &out_sample_in_bar;
    case outputnames::OUT_BEATS_PER_BAR:
        return &out_beats_per_bar;
    case outputnames::OUT_BEAT_VALUE:
        return &out_beat_value;
    case outputnames::OUT_BPM_CHANGE_TRIG:
        return &out_bpm_change_trig;
    case outputnames::OUT_METER_CHANGE_TRIG:
        return &out_meter_change_trig;
    case outputnames::OUT_BPM_CHANGE_STATE:
        return &out_bpm_change_state;
    default:
        return 0;
    }
}

dobj* timemap::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_METER:
        if (!(retv = add_meter_change((meterchange*)dbj)))
            *err_msg = "\ncould not add meter change to "
                       + *get_username();
        break;
    case dobjnames::SIN_BPM:
        if (!(retv = add_bpm_change((bpmchange*)dbj)))
            *err_msg = "\ncould not add bpm change to "
                       + *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to " + *get_username();
        retv = 0;
    }
    return retv;
}

synthmod* timemap::duplicate_module(const char* uname, DUP_IO dupio)
{
    *err_msg = "time_map module does not allow copies of it to be made.";
    return 0;
}

bool timemap::done_moddobj = false;
void timemap::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::TIMEMAP, dobjnames::LST_METER);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_METER, dobjnames::SIN_METER);
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::TIMEMAP, dobjnames::LST_BPM);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_BPM, dobjnames::SIN_BPM);
    done_moddobj = true;
}

#endif
