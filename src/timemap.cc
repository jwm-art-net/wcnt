#include "../include/timemap.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/jwm_init.h"

#include <sstream>

timemap::timemap(char const* uname) :

 synthmod(
    synthmodnames::TIMEMAP,
    uname,
    SM_UNGROUPABLE | SM_UNDUPLICABLE),

 out_bar(0), out_bar_trig(OFF), out_pos_in_bar(0), out_pos_step_size(0),
 out_bpm(0.0), out_sample_total(0), out_sample_in_bar(0),
 out_beats_per_bar(0), out_beat_value(0), out_bpm_change_trig(OFF),
 out_meter_change_trig(OFF), out_bpm_change_state(OFF), bpm_map(0),
 meter_map(0),  currentbpm(0), targetbpm(0), currentmeter(0),
 bpmsampletot(0), bpmchangesamp(0), bpmchange_pos(0), bpmrampsize(0),
 bpmchange_ratio(0), targbpm(0), pos_in_bar(0), bpmchange_notelen(0),
 bpmchangebar(0), barlength(0), beatlength(0), meterchangebar(0), p_bpm(0)
{
    register_output(outputnames::OUT_BPM);
    register_output(outputnames::OUT_BAR);
    register_output(outputnames::OUT_BAR_TRIG);
    register_output(outputnames::OUT_POS_IN_BAR);
    register_output(outputnames::OUT_POS_STEP_SIZE);
    register_output(outputnames::OUT_SAMPLE_TOTAL);
    register_output(outputnames::OUT_SAMPLE_IN_BAR);
    register_output(outputnames::OUT_BEATS_PER_BAR);
    register_output(outputnames::OUT_BEAT_VALUE);
    register_output(outputnames::OUT_BPM_CHANGE_TRIG);
    register_output(outputnames::OUT_METER_CHANGE_TRIG);
    register_output(outputnames::OUT_BPM_CHANGE_STATE);

    bpm_map = new linked_list<bpmchange>;
    meter_map = new linked_list<meterchange>;
    init_first();
}

timemap::~timemap()
{
    if (bpm_map)
        delete bpm_map;
    if (meter_map)
        delete meter_map;
}

bpmchange* timemap::add_bpm_change(bpmchange * bch)
{
    return (currentbpm =
        ordered_insert_replace(bpm_map, bch, &bpmchange::get_bar)
            ->get_data());
}

meterchange* timemap::add_meter_change(meterchange* mch)
{
    return (currentmeter =
        ordered_insert_replace(meter_map, mch, &meterchange::get_bar)
            ->get_data());
}

stockerrs::ERR_TYPE timemap::validate()
{
    if (!(currentmeter = meter_map->goto_first())) {
        *err_msg = "time signature not set for ";
        *err_msg += get_username();
        *err_msg += " - will not continue.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!(currentbpm = bpm_map->goto_first())) {
        *err_msg = "bpm not set for ";
        *err_msg += get_username();
        *err_msg += " - will not continue.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    std::ostringstream conv;
    if (currentmeter->get_bar() > 0) {
        *err_msg = "the first added time signature is at bar ";
        conv << currentmeter->get_bar();
        *err_msg += conv.str();
        *err_msg += " - should start at bar 0.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (currentbpm->get_bar() > 0) {
        *err_msg = "the first added tempo is at bar ";
        conv << currentbpm->get_bar();
        *err_msg += conv.str();
        *err_msg += " - should start at bar 0.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    double bpm = 0;
    while(currentbpm){
        bpm += currentbpm->get_bpm();
        if (bpm < jwm_init::min_bpm) {
            conv << "at bar " << currentbpm->get_bar();
            conv << " bpm change takes tempo below minimum value of ";
            conv << jwm_init::min_bpm;
            *err_msg = conv.str() + ".";
            invalidate();
            return stockerrs::ERR_ERROR;
        }
        if (bpm > jwm_init::max_bpm) {
            conv << "at bar " << currentbpm->get_bar();
            conv << " bpm change takes tempo above maximum value of ";
            conv << jwm_init::max_bpm;
            *err_msg = conv.str() + ".";
            invalidate();
            return stockerrs::ERR_ERROR;
        }
        currentbpm = bpm_map->goto_next();
    }
    return stockerrs::ERR_NO_ERROR;
}
#include <cstdio>
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

void timemap::init_first()
{
    if (done_first())
        return;
    register_moddobj(dobjnames::LST_METER, dobjnames::SIN_METER);
    register_moddobj(dobjnames::LST_BPM, dobjnames::SIN_BPM);
}

