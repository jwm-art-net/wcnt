/*--------------------------------------------------------------------------|
//  initially Created with Anjuta version 1.0.0, now edited in version 1.2.0|
//--------------------------------------------------------------------------|
//                                                                          |
//                     O---\\\\|||[:::>  <:::]|||////---O                   |
//                     \ ----------   wcnt   ---------- /                   |
//                          Wav Composer Not Toilet!                        |
//                     / ---------- v 1.1001 ---------- \                   |
//                     O---////|||[::>    <::]|||\\\\---O                   |
//                                     ||                                   |
//                          ========================                        |
//                          | 2004 James W. Morris.|                        |
//                          ------------------------                        |
//	                        |    Made In England   |                        |
//                          ------------------------                        |
//                                                                          /
// contact->JamesMorris( james@jwm-art.net )                               /
//                                                                        /
//-----------------------------------------------------------------------/


//-----------------------------------------------------------------------|
// This program is free software; you can redistribute it and/or modify  |
// it under the terms of the GNU General Public License as published by  |
// the Free Software Foundation; either version 2 of the License, or     /
// (at your option) any later version.                                  /
//                                                                     /
// This program is distributed in the hope that it will be useful,    /
// but WITHOUT ANY WARRANTY; without even the implied warranty of    /
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    /
// GNU Library General Public License for more details.            /
//                                                                 \__.
// You should have received a copy of the GNU General Public License  |
// along with this program; if not, write to the Free Software        |________
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*/
//                                                                           /
//---------------------------------------------_----------------------------/


//------------------------------------
// TESTING, TESTING, ONE, TWO, THREE:|
// ----------------------------------\______________________________________.
//                                                                         O|
// define BARE_MODULES here, before the include jwmsynth.h to eliminate code|
// which handles all the stuff which the modules don't actually need to work|
// useful for testing, testing, one, two, three                            O|
// well that's what I hoped would happen but it did not work. bugger.       /
// put it in src/Makefile.am instead:                                      /
//      -DBARE_MODULES                                                    /
// ----------------------------------+------------------------------------|.
// TESTING, TESTING, ONE, TWO, THREE:|
//-----------------------------------+

#ifdef BARE_MODULES
#include "../include/synthmodule.h"
#include "../include/modules.h"
#else
#include "../include/jwmsynth.h"
#endif

void title();
void exitramblings();
void memerr(const char* msg);

#ifdef BARE_MODULES	
void moduletest();
#endif

int main(int argc, char **argv)
{
	title();
	#ifdef BARE_MODULES
	moduletest();
	#else
	jwmsynth* thesynth = new jwmsynth(argc, argv);
	if (!thesynth->is_valid()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->scan_cl_options()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->generate_synth()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->connect_synth()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->execute_synth()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	#endif // BARE_MODULES
	exitramblings();
	return 0;
}

void title()
{
	cout << "\nO---\\\\\\\\|||[:::>  <:::]|||////---O";
	cout << "\n\\ ----------   wcnt   ---------- /";
	cout << "\n     Wav Composer Not Toilet!";
	cout << "\n/ ---------- v 1.1001 ---------- \\";
	cout << "\nO---////|||[::>    <::]|||\\\\\\\\---O\n";
}

void exitramblings()
{
	cout << "\nFlush complete.  Please wash your &&s.\n";
}

#ifdef BARE_MODULES
void moduletest()
{
	cout << "\nTesting, testing, one, two, three!";
	short exit_bar = 22;
	double* off = new double(0);
	double* one = new double(1);
	STATUS* trig_off = new STATUS(OFF);

	timemap* tmap = new timemap("timemap1");
	bpmchange* bc;
	tmap->add_bpm_change(1, 160);
	tmap->add_bpm_change(2, 120);
	tmap->add_bpm_change(3, 120);
	tmap->add_bpm_change(3, 160);
	tmap->add_bpm_change(4, 120);
	tmap->add_bpm_change(6, 130);
	tmap->add_bpm_change(8, 250);
	tmap->add_bpm_change(10, 999);// oh yes!
	tmap->add_bpm_change(10, 100);// oh I feel sick.....
	tmap->add_meter_change(1, 3, 4);
	tmap->add_meter_change(2, 7, 8);
	tmap->add_meter_change(3, 5, 2);
	tmap->add_meter_change(4, 6, 8);
	tmap->add_meter_change(5, 5, 4);
	tmap->add_meter_change(8, 4, 4);
	tmap->init();
	// bpmmap output capture
	const unsigned long* samp = tmap->get_out_sample_total();
	const STATUS* bartrig = tmap->get_out_bar_trig();
	const short* bar = tmap->get_out_bar();
	unsigned long const * barpos = tmap->get_out_pos_in_bar();
	const double* bpm = tmap->get_out_bpm();
	const double* posstpsz = tmap->get_out_pos_step_size();
	// Riffdata for sequencer	
	// riff1
	string riff1name = "seqtestriff1";
	riffdata* riff1 = new riffdata(riff1name);
//	note_data(const char* name, short len, short pos, double vel);
// using quarter_val = 12
// whole half quarter eighth sixteenth   32nd
//	48    24    12        6       3      1.5
	riff1->set_quartervalue(12);
	riff1->insert_and_position_note(new note_data("c0", 3.7, 0.0, 1.0));
	riff1->insert_and_position_note(new note_data("d0", 3.2, 6.0, 1.0));
	riff1->insert_and_position_note(new note_data("e0", 1.06, 12.0, 1.0));
	riff1->insert_and_position_note(new note_data("f0", 3.0, 18.0, 1.0));
	riff1->insert_and_position_note(new note_data("g0", 6.4, 24.2, 1.0));
	riff1->insert_and_position_note(new note_data("a0", 2.0, 30.0, 1.0));
	riff1->insert_and_position_note(new note_data("b0", 4.0, 36.5, 1.0));
	riff1->insert_and_position_note(new note_data("c1", 1.4, 42.0, 1.0));
	//riff2
	string riff2name = "seqtestriff2";
	riffdata* riff2 = new riffdata(riff2name);
	riff2->set_quartervalue(4);
	riff2->insert_and_position_note(new note_data("c-1", 64, 0, 1.0));
	string riff3name = "seqtestriff3";
	riffdata* riff3 = new riffdata(riff2name);
	riff3->set_quartervalue(12);
	riff3->insert_and_position_note(new note_data("c0", 192, 0, 1.0));
	string riff4name = "samptestriff1";
	riffdata* riff4 = new riffdata(riff4name);
	riff4->set_quartervalue(12);
	riff4->insert_and_position_note(new note_data("c-4", 11, 0, 1.0));
	riff4->insert_and_position_note(new note_data("c-3", 11, 12, 1.0));
	riff4->insert_and_position_note(new note_data("c-2", 11, 24, 1.0));
	riff4->insert_and_position_note(new note_data("c-1", 11, 36, 1.0));
	riff4->insert_and_position_note(new note_data("c0", 11, 48, 1.0));
	riff4->insert_and_position_note(new note_data("c1", 11, 60, 1.0));
	riff4->insert_and_position_note(new note_data("c2", 11, 72, 1.0));
	riff4->insert_and_position_note(new note_data("c3", 11, 84, 1.0));
	// sequencer
	string seqname = "seq1";
	sequencer* seq1 = new sequencer(seqname);
	seq1->add_riff(riff4, 0);
//	seq1->add_riff(riff1, 1);
	seq1->add_riff(riff1, 2);
	seq1->add_riff(riff1, 3);
	seq1->add_riff(riff1, 4);
	seq1->add_riff(riff1, 5);
	seq1->add_riff(riff1, 6);
	seq1->add_riff(riff1, 7);
	seq1->add_riff(riff1, 8);
	seq1->add_riff(riff1, 9);
	seq1->add_riff(riff2, 10);
	seq1->set_input_bar_trig(bartrig);
	seq1->set_input_bar(bar);
	seq1->set_input_pos_step_size(posstpsz);
	seq1->init();
	// sequencer
	string seq2name = "seq2";
	sequencer* seq2 = new sequencer(seqname);
	seq2->add_riff(riff4, 0);
//	seq2->add_riff(riff1, 1);
	seq2->add_riff(riff1, 2);
//	seq2->add_riff(riff2, 3);
	seq2->add_riff(riff2, 4);
//	seq2->add_riff(riff2, 5);
	seq2->add_riff(riff3, 6);
//	seq2->add_riff(riff3, 7);
//	seq2->add_riff(riff1, 8);
//	seq2->add_riff(riff2, 9);
	seq2->add_riff(riff1, 10);
	seq2->add_riff(riff1, 11);
	seq2->add_riff(riff3, 12);
//	seq2->add_riff(riff1, 13);
//	seq2->add_riff(riff1, 10);
//	seq2->add_riff(riff2, 11);
	seq2->set_input_bar_trig(bartrig);
	seq2->set_input_bar(bar);
	seq2->set_input_pos_step_size(posstpsz);
	seq2->init();
	// seq1 output capture
	const STATUS* note_on = seq1->get_output_note_on_trig();
	const STATUS* note_slide = seq1->get_output_note_slide_trig();
	const STATUS* note_off = seq1->get_output_note_off_trig();
	const STATUS* riff_start = seq1->get_output_riff_start_trig();
	const STATUS* riff_end = seq1->get_output_riff_end_trig();
	const STATUS* seq_end = seq1->get_output_end_trig();
	const double* freq = seq1->get_output_freq();
	const double* velocity = seq1->get_output_velocity();
	const double* velocity_ramped = seq1->get_output_velocity_ramp();
	const STATUS* riff_play = seq1->get_riff_play_state();
	const STATUS* note_play = seq1->get_note_play_state();
	// lfo_clock
	string lfo1name = "lfoclock1";
	lfo_clock* lfo1 = new lfo_clock(lfo1name);
	lfo1->set_input_phase_trig(trig_off);
	lfo1->set_input_freq_mod1(off);
	lfo1->set_input_freq_mod2(off);
	lfo1->set_freq_mod1size(0);
	lfo1->set_freq_mod2size(0);
	lfo1->set_frequency(0.1);
	lfo1->init();
	// sinwave for lfo
	string lfosinname = "lfo sinewave";
	sine_wave* lfosin = new sine_wave(lfosinname);
	lfosin->set_input_phase_trig(lfo1->get_output_phase_trig());
	lfosin->set_input_deg_size(lfo1->get_output_deg_size());
	lfosin->set_recycle_mode(OFF);
	lfosin->set_on_trig_zero_deg(OFF);
	lfosin->set_cycles(1.0);
	lfosin->init();
	// adsr
	string adsr1name = "adsr1";
	adsr* adsr1 = new adsr(adsr1name);
	// upper time, upper level, lower time, lower level
	adsr1->insert_coord(adsr_coord::ADSR_ATTACK, 	5, 0.6, 10, 0.5);
	adsr1->insert_coord(adsr_coord::ADSR_ATTACK, 	15, 1.0, 20, 0.9);
	adsr1->insert_coord(adsr_coord::ADSR_DECAY, 	115, 0.75, 115, 0.7);
	adsr1->insert_coord(adsr_coord::ADSR_DECAY, 	225, 0.55, 222, 0.4);
	adsr1->insert_coord(adsr_coord::ADSR_RELEASE, 	735, 0.5, 725, 0.35);
	adsr1->insert_coord(adsr_coord::ADSR_RELEASE, 	810, 0.0, 885, 0.0);
	adsr1->set_input_velocity(velocity);
	adsr1->set_input_note_on_trig(note_on);
	adsr1->set_input_note_off_trig(note_off);
	adsr1->set_start_level(0);
	adsr1->set_sustain_status(OFF);
	adsr1->set_zero_retrigger_mode(OFF);
	adsr1->init();
	string adsr2name = "adsr2";
	adsr* adsr2 = new adsr(adsr2name);
	// upper time, upper level, lower time, lower level
	adsr2->insert_coord(adsr_coord::ADSR_ATTACK, 	5, 1.0, 8, 0.95);
	adsr2->insert_coord(adsr_coord::ADSR_DECAY, 	145, 0.75, 145, 0.7);
	adsr2->insert_coord(adsr_coord::ADSR_DECAY, 	155, 0.25, 152, 0.24);
	adsr2->insert_coord(adsr_coord::ADSR_RELEASE, 	165, 0.15, 165, 0.15);
	adsr2->insert_coord(adsr_coord::ADSR_RELEASE, 	90, 0.0, 90, 0.0);
	adsr2->set_input_velocity(velocity);
	adsr2->set_input_note_on_trig(seq2->get_output_note_on_trig());//note_on);
	adsr2->set_input_note_off_trig(seq2->get_output_note_off_trig());//note_off);
	adsr2->set_start_level(0);
	adsr2->set_sustain_status(ON);
	adsr2->set_zero_retrigger_mode(ON);
	adsr2->init();
	string adsr3name = "adsr3";
	adsr* adsr3 = new adsr(adsr3name);
	// upper time, upper level, lower time, lower level
	adsr3->insert_coord(adsr_coord::ADSR_ATTACK, 	75, 1.0, 88, 0.95);
	adsr3->insert_coord(adsr_coord::ADSR_DECAY, 	145, 0.75, 145, 0.7);
	adsr3->insert_coord(adsr_coord::ADSR_DECAY, 	155, 0.55, 152, 0.53);
	adsr3->insert_coord(adsr_coord::ADSR_RELEASE, 	165, 0.15, 165, 0.15);
	adsr3->insert_coord(adsr_coord::ADSR_RELEASE, 	90, 0.0, 90, 0.0);
	adsr3->set_input_velocity(velocity);
	adsr3->set_input_note_on_trig(seq2->get_output_note_on_trig());
	adsr3->set_input_note_off_trig(seq2->get_output_note_off_trig());
	adsr3->set_start_level(0);
	adsr3->set_sustain_status(ON);
	adsr3->set_zero_retrigger_mode(OFF);
	adsr3->init();
	// osc_clock
	string osc1name = "osc_clock1";
	osc_clock* osc1 = new osc_clock(osc1name);
	osc1->set_input_note_on_trig(note_on);
	osc1->set_input_note_slide_trig(note_slide);
	osc1->set_input_freq(freq);
	osc1->set_input_freq_mod1(off);
	osc1->set_input_freq_mod2(off);
	osc1->set_freq_mod1size(1.0);
	osc1->set_freq_mod2size(1.0);
	osc1->set_octave(0);
	osc1->set_tuning_semitones(0);
	osc1->set_portamento(250);
	osc1->init();
	// sinwave
	string sin1name = "sinewave1";
	sine_wave* sin1 = new sine_wave(sin1name);
	sin1->set_input_phase_trig(osc1->get_output_phase_trig());
	sin1->set_input_deg_size(osc1->get_output_deg_size());
	sin1->set_recycle_mode(OFF);
	sin1->set_on_trig_zero_deg(OFF);
	sin1->set_cycles(1.0);
	sin1->init();
	// osc_clock
	string osc2name = "osc_clock2";
	osc_clock* osc2 = new osc_clock(osc2name);
	osc2->set_input_note_on_trig(seq2->get_output_note_on_trig());
	osc2->set_input_note_slide_trig(seq2->get_output_note_slide_trig());
	osc2->set_input_freq(seq2->get_output_freq());
	osc2->set_input_freq_mod1(off);
	osc2->set_input_freq_mod2(off);
	osc2->set_freq_mod1size(1.0);
	osc2->set_freq_mod2size(1.0);
	osc2->set_octave(0);
	osc2->set_tuning_semitones(0);
	osc2->set_portamento(250);
	osc2->init();
	// contraster
	string con1name = "contraster1";
	contraster* con1 = new contraster(con1name);
	con1->set_input_signal(sin1->get_output());
	con1->set_input_power_mod(adsr2->get_output());
	con1->set_input_rude_switch_trig(osc1->get_output_phase_trig());
	con1->set_power_min(1.0); // < 1 makes quieter.
	con1->set_power_max(1.0); // > 1 makes louder/contrast
	con1->set_rude_mode(OFF);
	con1->set_wetdry(0.25);// 1=wet, 0=dry
	con1->init();
	// mono_amp
	string amp1name = "amp1";
	mono_amp* amp1 = new mono_amp(amp1name);
	amp1->set_input_signal(con1->get_output());
	amp1->set_input_amp_eg(adsr1->get_output());
	amp1->set_input_amp_mod(off);
	amp1->set_input_pan_mod(off);
	amp1->set_amplitude(32500);
	amp1->set_amp_modsize(0.0);
	amp1->set_pan(0.0);
	amp1->set_pan_modsize(0.0);
	amp1->set_clip_level(32767);
	amp1->init();
	// wavfileout
	string wav1name = "wav1";
	wavfileout* wav1 = new wavfileout(wav1name);
	wav1->set_input_bar_trig(bartrig);
	wav1->set_input_bar(bar);
	wav1->set_input_left_channel(amp1->get_output_left());
	wav1->set_input_right_channel(amp1->get_output_right());
	wav1->set_start_bar(0);
	wav1->set_end_bar(12);
	if (wav1->open_wav("metertest.wav") != WAV_STATUS_OPEN) {
		cout << "\nCould not open outputfile: testing.wav";
		exit(1);
	}
	wav1->init();
	// serialwavfileout
	string swav1name = "serialwav1";
	serialwavfileout* swav1 = new serialwavfileout(swav1name);
	swav1->set_input_bar_trig(bartrig);
	swav1->set_input_bar(bar);
	swav1->set_input_left_channel(amp1->get_output_left());
	swav1->set_input_right_channel(amp1->get_output_right());
	swav1->set_start_bar(1);
	swav1->set_end_bar(2); // I won't be too mean and create 65 wavs!
	swav1->set_wav_basename("serialtest");
	swav1->set_input_write_trig(note_on);
	swav1->set_input_stop_trig(trig_off);
	swav1->init();
	// wavfilein
	wavfilein* wavsamp = new wavfilein;
	wavsamp->open_wav("SLWATER1.WAV");
	unsigned long wavsamp_len = wavsamp->get_length();
	// sampler
	string samp1name = "samp1";
	sampler* samp1 = new sampler(samp1name);
	samp1->set_input_play_trig(seq2->get_output_note_on_trig());
	samp1->set_input_stop_trig(adsr3->get_output_off_trig());//trig_off);
	samp1->set_input_deg_size(osc2->get_output_deg_size());
	samp1->set_input_start_pos_mod(lfosin->get_output());
	samp1->set_wavfilein(wavsamp);
	samp1->set_play_dir(PLAY_FWD);
	samp1->set_play_mode(PLAY_BOUNCE);
	samp1->set_jump_mode(JUMP_PLAY_DIR);
	samp1->set_start_pos_min(wavsamp_len * 0.015);
	samp1->set_start_pos_max(wavsamp_len * 0.450);
	samp1->set_loop_begin(wavsamp_len * 0.1);//10925);
	samp1->set_loop_end(wavsamp_len * 0.5);//167731);
	samp1->set_loop_mode(LOOP_BI);
	samp1->set_loop_is_offset(ON);
	samp1->set_loop_bi_offset(50);
	samp1->set_anti_clip_samples(100);
	samp1->set_anti_clip_each_end(ON);
	samp1->set_zero_search_range(40);
	samp1->set_root_note("c0");
	samp1->set_degsize_amount(1);
	samp1->init();

// contraster
	string con2name = "contraster2";
	contraster* con2 = new contraster(con2name);
	con2->set_input_signal(samp1->get_output_l());
	con2->set_input_power_mod(adsr2->get_output());
	con2->set_input_rude_switch_trig(trig_off);
	con2->set_power_min(1.0); // < 1 makes quieter.
	con2->set_power_max(6.0); // > 1 makes louder/contrast
	con2->set_rude_mode(OFF);
	con2->set_wetdry(0.75);// 1=wet, 0=dry
	con2->init();
	
	// clock
	string clock1name = "clock1";
	clockclock* clock1 = new clockclock(clock1name);
	clock1->set_input_freq_mod1(adsr3->get_output());
	clock1->set_frequency(400);
	clock1->set_freq_mod1size(8);
	clock1->init();

	// clock
	string clock2name = "clock2";
	clockclock* clock2 = new clockclock(clock2name);
	clock2->set_input_freq_mod1(adsr3->get_output());
	clock2->set_frequency(250);
	clock2->set_freq_mod1size(6);
	clock2->init();
	
	string sin2name = "sinewave2";
	sine_wave* sin2 = new sine_wave(sin2name);
	sin2->set_input_phase_trig(clock2->get_output_phase_trig());
	sin2->set_input_deg_size(clock2->get_output_deg_size());
	sin2->set_recycle_mode(OFF);
	sin2->set_on_trig_zero_deg(OFF);
	sin2->set_cycles(1.0);
	sin2->init();

	// echofilter
	string echo1name = "echofilter1";
	echo* echo1 = new echo(echo1name);
	echo1->set_input_signal(con2->get_output());
	echo1->set_input_gain_mod(adsr3->get_output());
	echo1->set_input_feedback_mod(sin1->get_output());
	echo1->set_delay_time(363.0);
	echo1->set_gain_level(0.64);
	echo1->set_gain_modsize(1.0);
	echo1->set_feedback_level(0.64026);
	echo1->set_feedback_modsize(0.26);
	echo1->set_wetdry(0.5);
	echo1->init();

	string echo2name = "echofilter2";
	echo* echo2 = new echo(echo2name);
	echo2->set_input_signal(echo1->get_output_wet());//samp1->get_output_r());
	echo2->set_input_gain_mod(off);
	echo2->set_input_feedback_mod(sin2->get_output());
	echo2->set_delay_time(134.73);
	echo2->set_gain_level(0.64);
	echo2->set_gain_modsize(0.0);
	echo2->set_feedback_level(0.486263);
	echo2->set_feedback_modsize(0.75);
	echo2->set_wetdry(0.5);
	echo2->init();
/*
	// modifier
	string mod1name = "modifier1";
	modifier* mod1 = new modifier(mod1name);
	mod1->set_input_signal(echo2->get_output());
	mod1->set_input_modifier(echo1->get_output());
	mod1->set_modifier_func(modifier::TAN);
	mod1->set_func_value(30);
	mod1->set_modifier_size(1.0);
	mod1->set_min_output_value(-1.0);
	mod1->set_max_output_value(1.0);
	mod1->set_posneg_mirror(OFF);
	mod1->init();
*/
	// lpf
	string lpf1name = "lpf1";
	lpfilter* lpf1 = new lpfilter(lpf1name);
	lpf1->set_input_signal(echo2->get_output_wet());
//	lpf1->set_input_signal(con2->get_output());
	lpf1->set_input_cutoff_deg_size(clock2->get_output_deg_size());
	lpf1->set_input_feedback_mod(lpf1->get_output_wet());
	lpf1->set_feedback_level(0.425);
	lpf1->set_feedback_modsize(0.00);
	lpf1->set_wetdry(1.0);
	lpf1->init();	

	echo1->set_input_feedback(echo2->get_output());
	
	echo2->set_input_feedback(lpf1->get_output());

	string echo3name = "echo3";
	echo* echo3 = new echo(echo3name);
	echo3->set_input_signal(lpf1->get_output_wet());//samp1->get_output_r());
	echo3->set_input_gain_mod(adsr3->get_output());
	echo3->set_input_feedback_mod(sin1->get_output());
	echo3->set_delay_time(53.73);
	echo3->set_gain_level(0.64);
	echo3->set_gain_modsize(0.40);
	echo3->set_feedback_level(0.646263);
	echo3->set_feedback_modsize(0.85);
	echo3->set_wetdry(0.85);
	echo3->init();

//	lpf1->set_input_feedback(echo3->get_output());
	lpf1->set_input_feedback(lpf1->get_output_wet());

	// hpf
	string hpf1name = "hpf1";
	hpfilter* hpf1 = new hpfilter(hpf1name);
	hpf1->set_input_signal(echo3->get_output());
	hpf1->set_input_cutoff_deg_size(clock1->get_output_deg_size());
	hpf1->set_input_feedback(hpf1->get_output_wet());
	hpf1->set_input_feedback_mod(off);
	hpf1->set_feedback_level(0.30);
	hpf1->set_feedback_modsize(0.0);
	hpf1->set_wetdry(0.69);
	hpf1->init();	

	echo3->set_input_feedback(hpf1->get_output());

	string amp2name = "amp2";
	mono_amp* amp2 = new mono_amp(amp2name);
	amp2->set_input_signal(samp1->get_output_l());//echo1->get_output());//samp1->get_output_l());
//	amp2->set_input_signal(lpf1->get_output());//samp1->get_output_l());
	amp2->set_input_amp_eg(adsr3->get_output());//one);
	amp2->set_input_amp_mod(off);
	amp2->set_input_pan_mod(lfosin->get_output());
	amp2->set_amplitude(25000);
	amp2->set_amp_modsize(0.00);
	amp2->set_pan(0.0);
	amp2->set_pan_modsize(-0.95);
	amp2->set_clip_level(32767);
	amp2->init();

	string amp3name = "amp3";
	mono_amp* amp3 = new mono_amp(amp3name);
	amp3->set_input_signal(echo2->get_output());//samp1->get_output_l());
//	amp3->set_input_signal(lpf1->get_output());//samp1->get_output_l());
	amp3->set_input_amp_eg(one);
	amp3->set_input_amp_mod(off);
	amp3->set_input_pan_mod(lfosin->get_output());
	amp3->set_amplitude(25000);
	amp3->set_amp_modsize(0.00);
	amp3->set_pan(0.0);
	amp3->set_pan_modsize(0.95);
	amp3->set_clip_level(32767);
	amp3->init();

	string amp4name = "amp4";
	mono_amp* amp4 = new mono_amp(amp4name);
	amp4->set_input_signal(echo1->get_output());
	amp4->set_input_amp_eg(one);
	amp4->set_input_amp_mod(off);
	amp4->set_input_pan_mod(off);
	amp4->set_amplitude(25000);
	amp4->set_amp_modsize(0.0);
	amp4->set_pan(0.0);
	amp4->set_pan_modsize(0.0);
	amp4->set_clip_level(32767);
	amp4->init();

	string chan1name = "channel 1";
	stereo_channel* chan1 = new stereo_channel(chan1name);
	chan1->set_input_left(amp2->get_output_left());
	chan1->set_input_right(amp2->get_output_right());
	
	string chan2name = "channel 2";
	stereo_channel* chan2 = new stereo_channel(chan2name);
	chan2->set_input_left(amp3->get_output_left());
	chan2->set_input_right(amp3->get_output_right());

	string chan3name = "channel 3";
	stereo_channel* chan3 = new stereo_channel(chan3name);
	chan3->set_input_left(amp4->get_output_left());
	chan3->set_input_right(amp4->get_output_right());
	
	string mix1name = "mixer 1";
	stereomixer* mix1 = new stereomixer(mix1name);
	mix1->add_channel(chan1);
//	mix1->add_channel(chan2);
//	mix1->add_channel(chan3);
	mix1->set_master_level(0.95);

	// wavfileout
	string wav2name = "wav2";
	wavfileout* wav2 = new wavfileout(wav2name);
	wav2->set_input_bar_trig(bartrig);
	wav2->set_input_bar(bar);
	wav2->set_input_left_channel(mix1->get_output_left());
	wav2->set_input_right_channel(mix1->get_output_right());
	wav2->set_start_bar(0);
	wav2->set_end_bar(22);
	if (wav2->open_wav("sampletest.wav") != WAV_STATUS_OPEN) {
		cout << "\nCould not open outputfile: testing.wav";
		exit(1);
	}
	wav2->init();
	// so very nearly almost there....
	STATUS display_notes = OFF;
	unsigned long i = 0;
	int displayevery = 4096;
	// atlast:
	while(*bar != exit_bar) {
		tmap->run();
		seq1->run();
		seq2->run();
		lfo1->run();
		lfosin->run();
		adsr1->run();
		adsr2->run();
		adsr3->run();
		osc1->run();
		sin1->run();
		osc2->run();
		con1->run();
		amp1->run();
		wav1->run();
		swav1->run();
		samp1->run();
		con2->run();
		clock1->run();
		lpf1->run();
		clock2->run(); 
		sin2->run();
		echo1->run();
		echo2->run();
		echo3->run();
		hpf1->run();
	//	mod1->run();
		amp2->run();
		amp3->run();
		amp4->run();
		mix1->run();
		wav2->run();				
	if (*bartrig == ON) 
			cout << "\n\nBar " << *bar;
		if (display_notes == ON) {
			if (*note_off == ON)  
				cout << "\nNote OFF  at";
			if (*note_on == ON) 
				cout << "\nNote ON   ";
			if (*note_slide == ON)
				cout << "\nNote SLIDE";
			if (*note_on == ON || *note_slide == ON) {
				cout << "\tfreq     " << *freq;
				cout << "\tvelocity " << *velocity;
				cout << "\tat";
			}
			if (*note_off == ON || *note_on == ON
				|| *note_slide == ON) {
					cout << "\nsample " << *samp;
					cout << "\tpos    " << *barpos;
			}
		}
		if ((i % displayevery) == 0)  
		{
			cout << "\nsample " << *samp;
			cout << "\tpos    " << *barpos;
			cout << "\tbpm    " << *bpm;
			cout << "\tbpm change state ";
			if (*tmap->get_out_bpm_change_state() == OFF) 
				cout << "OFF";
			else
				cout << "ON";
		}
		i++;
	}
	delete one;
	delete off;
	delete tmap;
	delete seq1;
	delete seq2;
	delete lfo1;
	delete lfosin;
	delete adsr1;
	delete adsr2;
	delete adsr3;
	delete osc1;
	delete sin1;
	delete osc2;
	delete con1;
	delete amp1;
	delete wav1;
	delete swav1;
	delete samp1;
	delete con2;
	delete lpf1;
	delete amp2;
	delete wav2;
	cout << "\nDone testing...\n";
}
#endif
