#ifndef INPUTNAMES_H
#define INPUTNAMES_H

#include <string>
#include "iocat.h"

class inputnames
{
 public:
	enum IN_TYPE
	{	
		IN_FIRST, // <-- returned on error
		IN_VELOCITY,
		IN_NOTE_ON_TRIG,
		IN_NOTE_SLIDE_TRIG,
		IN_NOTE_OFF_TRIG,
		IN_LEFT,
		IN_RIGHT,
		IN_FREQ,
		IN_FREQ_MOD1,
		IN_FREQ_MOD2,
		IN_PHASE_TRIG,
		IN_DEG_SIZE,
		IN_SIGNAL,
		IN_EG,
		IN_AMP_MOD,
		IN_PAN_MOD,
		IN_TRIG,
		IN_WAVE,
		IN_PWM,
		IN_CUTOFF_DEG_SIZE,
		IN_RES_AMP_MOD,
		IN_PLAY_TRIG,
		IN_START_POS_MOD,
		IN_MODIFIER,
		IN_NORM_MOD,
		IN_V_MOD,
		IN_H_MOD,
		IN_TRIG1,
		IN_TRIG2,
		IN_LAST  // <-- used for array
	};
	inputnames();
	~inputnames();
	string getname(IN_TYPE);
	IOCAT getcategory(IN_TYPE);
	IN_TYPE get_type(string const*);
		
 private:
	string *inname;
	IOCAT *incat;
};

#endif
