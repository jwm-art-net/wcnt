#ifndef OUTPUTNAMES_H
#define OUTPUTNAMES_H

#include <string>
#include "iocat.h"

class outputnames {
 public:
	enum OUT_TYPE
	{	
		OUT_FIRST, // used in error situations also
		OUT_NONE_DOUBLE,
		OUT_NONE_SHORT,
		OUT_NONE_ULONG,
		OUT_NONE_TRIG,
		OUT_NONE_STATE,
		OUT_OUTPUT,
		OUT_PLAY_STATE,
 		OUT_LEFT,
		OUT_RIGHT,
		OUT_TRIG,
		OUT_PHASE_TRIG,
		OUT_PREMOD_DEG_SIZE,
		OUT_DEG_SIZE,
		OUT_FREQ,
		OUT_NOTE_ON_TRIG,
		OUT_NOTE_SLIDE_TRIG,
		OUT_NOTE_OFF_TRIG,
 		OUT_RIFF_START_TRIG,
		OUT_RIFF_END_TRIG,
 		OUT_START_TRIG,
		OUT_END_TRIG,
		OUT_VELOCITY, 		
		OUT_TOTAL_SAMPLE_POS,
		OUT_RIFF_PLAY_STATE, 	
		OUT_NOTE_PLAY_STATE,
		OUT_OFF_PULSE,
		OUT_L,
		OUT_R, 		
		OUT_WRITE_STATE,
		OUT_WRITE_START_TRIG,
		OUT_WRITE_END_TRIG,	
		OUT_PRE_AMP_MOD,
		OUT_NOT_TRIG,
		OUT_LAST // know when to stop
	};
	
	outputnames();
	~outputnames();
	string getname(OUT_TYPE) const;
	IOCAT getcategory(OUT_TYPE) const;
	OUT_TYPE get_type(string const*) const;
	OUT_TYPE get_nonezerotype(IOCAT);
		
 private:
	string *outname;
	IOCAT *outcat;
};

#endif
