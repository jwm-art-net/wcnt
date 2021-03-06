#include "../include/timemap.h"
#include "../include/globals.h"
#include "../include/listwork.h"

#include <sstream>

timemap::timemap(const char* uname) :
 synthmod::base(synthmod::TIMEMAP, uname, SM_UNGROUPABLE | SM_UNDUPLICABLE),
 out_bar(0), out_bar_trig(OFF), out_pos_in_bar(0), out_pos_step_size(0),
 out_bpm(0.0), out_sample_total(0), out_sample_in_bar(0),
 out_beats_per_bar(0), out_beat_value(0), out_bpm_change_trig(OFF),
 out_meter_change_trig(OFF), out_bpm_change_state(OFF), bpm_map(0),
 meter_map(0),  currentbpm(0), targetbpm(0), currentmeter(0),
 bpmsampletot(0), bpmchangesamp(0), bpmchange_pos(0), bpmrampsize(0),
 bpmchange_ratio(0), targbpm(0), pos_in_bar(0), bpmchange_notelen(0),
 bpmchangebar(0), barlength(0), beatlength(0), meterchangebar(0), p_bpm(0)
{
    register_output(output::OUT_BPM);
    register_output(output::OUT_BAR);
    register_output(output::OUT_BAR_TRIG);
    register_output(output::OUT_POS_IN_BAR);
    register_output(output::OUT_POS_STEP_SIZE);
    register_output(output::OUT_SAMPLE_TOTAL);
    register_output(output::OUT_SAMPLE_IN_BAR);
    register_output(output::OUT_BEATS_PER_BAR);
    register_output(output::OUT_BEAT_VALUE);
    register_output(output::OUT_BPM_CHANGE_TRIG);
    register_output(output::OUT_METER_CHANGE_TRIG);
    register_output(output::OUT_BPM_CHANGE_STATE);

    bpm_map = new linked_list<bpmchange>;
    meter_map = new linked_list<meterchange>;
}

void timemap::register_ui()
{
    register_dobj(dobj::LST_METER, dobj::SIN_METER);
    register_dobj(dobj::LST_BPM, dobj::SIN_BPM);
}

ui::moditem_list* timemap::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
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

errors::TYPE timemap::validate()
{
    if (!(currentmeter = meter_map->goto_first())) {
        sm_err("Time signature not set for %s. Will not continue.",
                                                    get_username());
        invalidate();
        return errors::ERROR;
    }
    if (!(currentbpm = bpm_map->goto_first())) {
        sm_err("BPM not set for %s. Will not continue.", get_username());
        invalidate();
        return errors::ERROR;
    }
    if (currentmeter->get_bar() > 0) {
        sm_err("The first added time signature is at bar %d. Should be "
                                    "at bar 0.", currentmeter->get_bar());
        invalidate();
        return errors::ERROR;
    }
    if (currentbpm->get_bar() > 0) {
        sm_err("The first added tempo is at bar %d. Should start at "
                                        "bar 0.", currentbpm->get_bar());
        invalidate();
        return errors::ERROR;
    }
    double bpm = 0;
    while(currentbpm){
        bpm += currentbpm->get_bpm();
        if (bpm < wcnt::min_bpm) {
            sm_err("At bar %d BPM change takes tempo below minimum "
                                    "BPM of %d.", currentbpm->get_bar(),
                                                    wcnt::min_bpm);
            invalidate();
            return errors::ERROR;
        }
        if (bpm > wcnt::max_bpm) {
            sm_err("At bar %d BPM change takes tempo above maximum "
                                    "BPM of %d.", currentbpm->get_bar(),
                                                    wcnt::max_bpm);
            invalidate();
            return errors::ERROR;
        }
        currentbpm = bpm_map->goto_next();
    }
    return errors::NO_ERROR;
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
    beatlength = (wcint_t)(TPQN * (4.0 / (float)out_beat_value));
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
            beatlength = (wcint_t)(TPQN * (4.0 / (double)out_beat_value));
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
                bpmchange_notelen = (samp_t)
                    ((bpmchangebar - currentbpm->get_bar()) * barlength);
                targbpm = p_bpm + targetbpm->get_bpm();
                // these will change during bpm ramp
                bpmchange_pos = 0;
                bpmsampletot = notelen_to_samples(bpmchange_notelen);
                bpmrampsize = (targbpm - out_bpm) / (double) bpmsampletot;
                out_bpm_change_state = ON;
            }
        }
        out_pos_step_size = barlength / (wcnt::jwm.samplerate() *
         (60.0 / out_bpm) * out_beats_per_bar);
    }
    else {
        if (out_bpm_change_trig == ON) out_bpm_change_trig = OFF;
        if (bpmrampsize != 0) {
            if (out_meter_change_trig == ON) {// there's always one!
                bpmchange_notelen = (samp_t)
                    ((bpmchangebar - out_bar) * barlength);
                bpmchange_pos = 0;
                bpmsampletot = notelen_to_samples(bpmchange_notelen);
                bpmrampsize = (targbpm - out_bpm) / (double) bpmsampletot;
                out_pos_step_size = barlength / (wcnt::jwm.samplerate()
                                  * (60.0 / out_bpm) * out_beats_per_bar);
            }
            out_bpm += bpmrampsize;
            bpmchange_ratio = (double) bpmchange_pos / bpmchange_notelen;
            bpmsampletot = (samp_t)(wcnt::jwm.samplerate() * (60 / out_bpm)
                         * ((double) (bpmchange_notelen - bpmchange_pos) 
                         / beatlength));
            bpmrampsize = (targbpm - out_bpm) / (double) bpmsampletot;
            out_pos_step_size = barlength / (wcnt::jwm.samplerate()
                              * (60 / out_bpm) * out_beats_per_bar);
            bpmchange_pos += out_pos_step_size;
        } else out_bpm_change_state = OFF;
    }
    out_sample_total++;
    out_sample_in_bar++;
    out_pos_in_bar = (samp_t)pos_in_bar;
    pos_in_bar += out_pos_step_size;
}

samp_t timemap::notelen_to_samples(wcint_t nl) const
{
    return (samp_t)(wcnt::jwm.samplerate()
     * ((double) 60 / out_bpm) * ((double)nl / beatlength));
}

#ifdef UNUSED
samp_t timemap::ms_to_samples(double ms) const
{
    return (samp_t)(wcnt::jwm.samplerate() * (ms / 1000));
}

double timemap::notelen_to_frequency(wcint_t nl) const
{
    return 1000 / (((double)60 / out_bpm) *
                   ((double)nl / beatlength) * 1000);
}

double timemap::notelen_to_ms(wcint_t nl) const
{
    return ((double)60 / out_bpm) * ((double)nl / beatlength) * 1000;
}
#endif

const void* timemap::get_out(output::TYPE ot) const
{
    switch (ot) {
    case output::OUT_BPM:
        return &out_bpm;
    case output::OUT_BAR:
        return &out_bar;
    case output::OUT_BAR_TRIG:
        return &out_bar_trig;
    case output::OUT_POS_IN_BAR:
        return &out_pos_in_bar;
    case output::OUT_POS_STEP_SIZE:
        return &out_pos_step_size;
    case output::OUT_SAMPLE_TOTAL:
        return &out_sample_total;
    case output::OUT_SAMPLE_IN_BAR:
        return &out_sample_in_bar;
    case output::OUT_BEATS_PER_BAR:
        return &out_beats_per_bar;
    case output::OUT_BEAT_VALUE:
        return &out_beat_value;
    case output::OUT_BPM_CHANGE_TRIG:
        return &out_bpm_change_trig;
    case output::OUT_METER_CHANGE_TRIG:
        return &out_meter_change_trig;
    case output::OUT_BPM_CHANGE_STATE:
        return &out_bpm_change_state;
    default:
        return 0;
    }
}

dobj::base* timemap::add_dobj(dobj::base* dbj)
{
    dobj::base* retv = 0;
    dobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobj::SIN_METER:
        if (!(retv = add_meter_change((meterchange*)dbj)))
            sm_err("Could not add meter change to %s.", get_username());
        break;
    case dobj::SIN_BPM:
        if (!(retv = add_bpm_change((bpmchange*)dbj)))
            sm_err("Could not add bpm change to %s.", get_username());
        break;
    default:
        sm_err("%s %s to %s.", errors::stock::major, errors::stock::bad_add,
                                                      get_username());
        retv = 0;
    }
    return retv;
}

synthmod::base* timemap::duplicate_module(const char* uname, DUP_IO dupio)
{
    (void)uname; (void)dupio; // stop unused param warns
    sm_err("%s", "The time_map module does not allow duplication.");
    return 0;
}

