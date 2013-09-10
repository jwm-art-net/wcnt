#ifndef OUTPUTNAMES_H
#include "../include/outputnames.h"

outputnames::outputnames()
: outname(NULL), outcat(NULL)
{
	outname = new string[OUT_LAST + 1];
	outcat = new IOCAT[OUT_LAST + 1];
	if (outname == NULL || outcat == NULL)
	{
		cout << "\nsevere lack of memory, exiting.....";
		cout << "\ncould not create list of output names and categories.";
		exit(1);
	}
	outname[OUT_FIRST] = "BAD_OUTPUT_TYPE";			
	outcat[OUT_FIRST] = CAT_FIRST;
	outname[OUT_NONE_DOUBLE] = "off";					
	outcat[OUT_NONE_DOUBLE] = CAT_DOUBLE;
	outname[OUT_NONE_SHORT] = "off";	
	outcat[OUT_NONE_SHORT] = CAT_SHORT;
	outname[OUT_NONE_ULONG] = "off";	
	outcat[OUT_NONE_ULONG] = CAT_ULONG;
	outname[OUT_NONE_TRIG] = "off";		
	outcat[OUT_NONE_TRIG] = CAT_TRIG;
	outname[OUT_NONE_STATE] = "off";		
	outcat[OUT_NONE_STATE] = CAT_STATE;
	outname[OUT_OUTPUT] = "out_output";			
	outcat[OUT_OUTPUT] = CAT_DOUBLE;
	outname[OUT_PLAY_STATE] = "out_play_state";	
	outcat[OUT_PLAY_STATE] = CAT_STATE;
	outname[OUT_LEFT] = "out_left";		
	outcat[OUT_LEFT] = CAT_SHORT;
	outname[OUT_RIGHT] = "out_right";	
	outcat[OUT_RIGHT] = CAT_SHORT;
	outname[OUT_TRIG] = "out_trig";	
	outcat[OUT_TRIG] = CAT_TRIG;
	outname[OUT_PHASE_TRIG] = "out_phase_trig";		
	outcat[OUT_PHASE_TRIG] = CAT_TRIG;
	outname[OUT_PREMOD_DEG_SIZE] = "out_premod_degsize";
	outcat[OUT_PREMOD_DEG_SIZE] = CAT_DOUBLE;
	outname[OUT_DEG_SIZE] = "out_deg_size";			
	outcat[OUT_DEG_SIZE] = CAT_DOUBLE;
	outname[OUT_FREQ] = "out_frequency";					
	outcat[OUT_FREQ] = CAT_DOUBLE;
	outname[OUT_NOTE_ON_TRIG] = "out_note_on_trig";	
	outcat[OUT_NOTE_ON_TRIG] = CAT_TRIG;
	outname[OUT_NOTE_SLIDE_TRIG] = "out_note_slide_trig";	
	outcat[OUT_NOTE_SLIDE_TRIG] = CAT_TRIG;
	outname[OUT_NOTE_OFF_TRIG] = "out_note_off_trig";		
	outcat[OUT_NOTE_OFF_TRIG] = CAT_TRIG;
	outname[OUT_RIFF_START_TRIG] = "out_riff_start_trig";		
	outcat[OUT_RIFF_START_TRIG] = CAT_TRIG;	
	outname[OUT_RIFF_END_TRIG] = "out_riff_end_trig";			
	outcat[OUT_RIFF_END_TRIG] = CAT_TRIG;
	outname[OUT_START_TRIG] = "out_start_trig";				
	outcat[OUT_START_TRIG] = CAT_TRIG;
	outname[OUT_END_TRIG] = "out_end_trig";					
	outcat[OUT_END_TRIG] = CAT_TRIG;
	outname[OUT_VELOCITY] = "out_velocity";					
	outcat[OUT_VELOCITY] = CAT_DOUBLE;
	outname[OUT_TOTAL_SAMPLE_POS] = "out_total_sample_pos";
	outcat[OUT_TOTAL_SAMPLE_POS] = CAT_ULONG;
	outname[OUT_RIFF_PLAY_STATE] = "out_riff_play_state";	
	outcat[OUT_RIFF_PLAY_STATE] = CAT_STATE;
	outname[OUT_NOTE_PLAY_STATE] = "out_note_play_state";
	outcat[OUT_NOTE_PLAY_STATE] = CAT_STATE;
	outname[OUT_OFF_PULSE] = "out_pulse_off_trig";		
	outcat[OUT_OFF_PULSE] = CAT_TRIG;
	outname[OUT_L] = "out_l";							
	outcat[OUT_L] = CAT_DOUBLE;
	outname[OUT_R] = "out_r";							
	outcat[OUT_R] = CAT_DOUBLE;
	outname[OUT_WRITE_STATE] = "out_file_write_state";		
	outcat[OUT_WRITE_STATE] = CAT_STATE;
	outname[OUT_WRITE_START_TRIG] = "out_write_start_trig";	
	outcat[OUT_WRITE_START_TRIG] = CAT_TRIG;
	outname[OUT_WRITE_END_TRIG] = "out_write_end_trig";		
	outcat[OUT_WRITE_END_TRIG] = CAT_TRIG;
	outname[OUT_PRE_AMP_MOD] = "out_pre_amp_mod";			
	outcat[OUT_PRE_AMP_MOD] = CAT_DOUBLE;
	outname[OUT_NOT_TRIG] = "out_not_trig";
	outcat[OUT_NOT_TRIG] = CAT_TRIG;
}

outputnames::~outputnames()
{
	if (outname) 
		delete[] outname;
	if (outcat)
		delete[] outcat;
}

string outputnames::getname(OUT_TYPE id) const
{
	if (id >= OUT_FIRST && id < OUT_LAST) 
		return outname[id];
	else 
		return outname[OUT_FIRST];
}

IOCAT outputnames::getcategory(OUT_TYPE id) const
{
	if (id >= OUT_FIRST && id < OUT_LAST)
		return outcat[id];
	else
		return outcat[OUT_FIRST];
}

outputnames::OUT_TYPE outputnames::get_type(string const* oname) const
{
	int i;
	for (i = OUT_FIRST + 1; i < OUT_LAST; i++)
		if (outname[i] == *oname)
			return (OUT_TYPE)i;
	return OUT_FIRST;
}

outputnames::OUT_TYPE outputnames::get_nonezerotype(IOCAT iocat){
	OUT_TYPE ot;
	switch(iocat)
	{
		case CAT_DOUBLE:
			ot = OUT_NONE_DOUBLE;
			break;
		case CAT_SHORT:
			ot = OUT_NONE_SHORT;			
			break;
		case CAT_ULONG:
			ot = OUT_NONE_ULONG;
			break;
		case CAT_TRIG:
			ot = OUT_NONE_TRIG;
			break;
		case CAT_STATE: 
			ot = OUT_NONE_STATE;
			break;
		default:
			ot = OUT_FIRST;
	}
	return ot;
}

#endif
