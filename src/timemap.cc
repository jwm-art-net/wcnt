#ifndef TIMEMAP_H
#include "../include/timemap.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

timemap::timemap(char const* uname) :

 synthmod(
    synthmodnames::TIMEMAP,
    uname,
    SM_UNGROUPABLE | SM_UNDUPLICABLE),

 out_bar(0), out_pos_in_bar(0), out_pos_step_size(0), out_bpm(0.0),
 out_sample_total(0), out_sample_in_bar(0),
 bpm_map(0), meter_map(0),
 currentbpm(0), targetbpm(0),
 currentmeter(0),
 bpmsampletot(0), bpmchangesamp(0),
 bpmchange_pos(0), bpmrampsize(0), bpmchange_ratio(0), targbpm(0),
 pos_in_bar(0), bpmchange_notelen(0), bpmchangebar(0)
{
    jwm.get_outputlist()->add_output(this,outputnames::OUT_BPM);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_BAR);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_BAR_TRIG);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_POS_IN_BAR);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_POS_STEP_SIZE);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_SAMPLE_TOTAL);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_SAMPLE_IN_BAR);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_BEATS_PER_BAR);
    jwm.get_outputlist()->add_output(this,outputnames::OUT_BEAT_VALUE);
    jwm.get_outputlist()->add_output(this,
                                    outputnames::OUT_BPM_CHANGE_TRIG);
    jwm.get_outputlist()->add_output(this,
                                    outputnames::OUT_METER_CHANGE_TRIG);
    jwm.get_outputlist()->add_output(this,
                                    outputnames::OUT_BPM_CHANGE_STATE);

    bpm_map = new linked_list<bpmchange>;
    if (!bpm_map->add_at_head(new bpmchange(0, 0)))
        invalidate();
    meter_map = new linked_list<meterchange>;
    if (!meter_map->add_at_head(
        new meterchange(
            0,
            jwm.beats_per_measure(),
            jwm.beat_value())))
        invalidate();
    create_moddobj();
}

timemap::~timemap()
{
    if (bpm_map)
        delete bpm_map;
    if (meter_map)
        delete meter_map;
}

bpmchange* timemap::add_bpm_change(short atbar, double bpm)
{
    bpmchange* newbpm = new bpmchange(atbar, bpm);
    if (!(currentbpm = add_bpm_change(newbpm))) {
        delete newbpm;
        return 0;
    }
    return newbpm;
}

bpmchange* timemap::add_bpm_change(bpmchange * bch)
{
    currentbpm = bpm_map->goto_first();
    if (bch->get_bar() == 0) {
        delete currentbpm;
        bpm_map->sneak_current()->set_data(bch);
        return (currentbpm = bch);
    }
    while(currentbpm){
        bpmchange* bc = bpm_map->sneak_next()->get_data();
        if (currentbpm->get_bar() == bch->get_bar()) {
            if (!bc) { // currentbpm is last in list - safe to add
                return currentbpm =
                    bpm_map->add_at_tail(bch)->get_data();
            }
            if (bc->get_bar() == bch->get_bar())
                return 0;
            if (bc->get_bar() > bch->get_bar()) {
                return currentbpm =
                    bpm_map->insert_after(
                        bpm_map->sneak_current(), bch
                    )->get_data();
            }
        }
        if (bc) {
            if (bc->get_bar() > bch->get_bar()) { // insert after current
                return currentbpm =
                    bpm_map->insert_after(
                        bpm_map->sneak_current(), bch
                    )->get_data();
            }
        }
        else
            return currentbpm = bpm_map->add_at_tail(bch)->get_data();
        currentbpm = bpm_map->goto_next();
    }
    return 0;  // should not ever get here..
}

meterchange* timemap::add_meter_change(short atbar, char btpb, char btval)
{
    meterchange* newmeter = new meterchange(atbar, btpb, btval);
    if (!(currentmeter = add_meter_change(newmeter))){
        delete newmeter;
        return 0;
    }
    return newmeter;
}

meterchange* timemap::add_meter_change(meterchange* mch)
{
    currentmeter = meter_map->goto_first();
    // don't forget the list will *never* be empty
    if (mch->get_bar() == 0) {
        delete currentmeter;
        meter_map->sneak_first()->set_data(mch);
        return currentmeter = mch;
    }
    while(currentmeter){
        if (currentmeter->get_bar() == mch->get_bar()) {
            delete currentmeter;
            meter_map->sneak_current()->set_data(mch);
            return (currentmeter = mch);
        }
        meterchange* mc = meter_map->sneak_next()->get_data();
        if (mc) {
            if (mc->get_bar() > mch->get_bar()) {
                return currentmeter =
                    meter_map->insert_after(
                        meter_map->sneak_current(), mch)
                            ->get_data();
            }
        }
        else {
            
            return currentmeter =
                meter_map->add_at_tail(mch)->get_data();
        }
        currentmeter = meter_map->goto_next();
    }
    return 0;
}

void timemap::init()
{
    currentbpm = bpm_map->goto_first();
    bpmchangebar = currentbpm->get_bar(); // = 0;
    targetbpm = currentbpm;
    currentmeter = meter_map->goto_first();
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
            currentmeter = meter_map->goto_next();
            if (!currentmeter)
                meterchangebar = -1;
            else
                meterchangebar = currentmeter->get_bar();
        }
    }
    else
    if (out_meter_change_trig == ON)
        out_meter_change_trig = OFF;
    if (out_bar == bpmchangebar) {
        currentbpm = targetbpm;
        out_bpm = p_bpm + currentbpm->get_bpm();
        p_bpm = out_bpm;
        targetbpm = bpm_map->goto_next();
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
                targetbpm = bpm_map->goto_next();
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
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::TIMEMAP, dobjnames::LST_METER);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_METER, dobjnames::SIN_METER);
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::TIMEMAP, dobjnames::LST_BPM);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_BPM, dobjnames::SIN_BPM);
    done_moddobj = true;
}

#endif
