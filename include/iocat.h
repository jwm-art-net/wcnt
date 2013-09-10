#ifndef IOCAT_H
#define IOCAT_H

// Used by a huge evergrowing pulsating brain that rules the world from the center of the ultraworld.

enum IOCAT 
{ 
	// among other things, error category:
	CAT_FIRST,
	// the following types cover all inputs and outputs:
	CAT_DOUBLE,
	CAT_SHORT,
	CAT_ULONG,
	CAT_TRIG,
	CAT_STATE, 
	// these have been added as extra types for parameter 
	// names and won't be used by inputs or outputs:
	CAT_FILENAME,	// errm uh dunno.
	CAT_NOTENAME,	// sea sharp naught or c#0 
	CAT_MOD_FUNC,	// enum modifier::MOD_FUNC
	CAT_CLIP_MODE,	// enum sampler::CLIP_MODE
	CAT_LOOP_MODE,	// enum sampler::LOOP_MODE
	CAT_WAVFILEIN,	// class wavfilein
	CAT_LOGIC,		// enum logictrigger::LOGIC_FUNC
	// keep last:
	CAT_LAST
};

/* 
	CAT_TRIG and CAT_STATE both refer to enum STATUS { OFF, ON } defined in
	synthmodule.h.  The difference being CAT_TRIG is only ON for the duration
	of one sample  whereas CAT_STATE is ON for as long as it takes.
	ie many samples.
	The distinction is made because it does not make any sense for a play_state
	output to plug into a trigger input which would cause continual retrigger on every
	sample until playstate was turned off.
	CAT_FILENAME, CAT_MOD_FUNC, CAT_CLIP_MODE were added for a 
	handful of parameters which do not use the normal types.
*/

#endif
