#ifndef IOCAT_H
#define IOCAT_H

#ifndef BARE_MODULES

#include <string>

// Used by triggers and switches and states 
enum STATUS { OFF, ON };

// every input, output, and parameter, will be of a category from this list

enum IOCAT 
{ 
	// keep first:
	CAT_FIRST,
	//----------------------------------------------------------------
	// the following types cover all inputs and outputs:
	//----------------------------------------------------------------
	CAT_DOUBLE, 	// signal, bpm, etc
	CAT_SHORT,  	// amplitude, etc
	CAT_ULONG,  	// sample positions etc
	CAT_TRIG,   	// trigger 		- ON/OFF - on for one sample only
	CAT_STATE,  	// power/status	- ON/OFF 
	//----------------------------------------------------------------
	// these have been added as extra types for parameter 
	// names and won't be used by inputs or outputs:
	// note: above statement is subject to change...
	//----------------------------------------------------------------
	CAT_STRING,		// notename, filename, etc
	CAT_MOD_FUNC,	// enum modifier::MOD_FUNC
	CAT_LOGIC,		// enum logictrigger::LOGIC_FUNC
	CAT_LOOP_MODE,	// enum LOOP_MODE
	CAT_PLAY_DIR,	// enum PLAY_DIR 
	CAT_PLAY_MODE,	// enum PLAY_MODE
	CAT_JUMP_DIR,	// enum JUMP_DIR
	CAT_METER,		// time signature  ie 4/4 3/4 etc
	CAT_ADSRSECT,	// enum adsr_coord::SECT
	// the following types are read as strings but they need to match 
	// the name of module/object 
	CAT_DOBJ,		// looks up data from dobjlist
	CAT_SYNTHMOD,	// looks up data from smodlist
	// cat dobj and cat synthmod have arisen because of new code to 
	// read data types ie riffs, adsrs, envelopes, tracks etc rather
	// than it being specialistically hard coded as before.
	//----------------------------------------------------------------
	// keep last:
	CAT_LAST
};

class iocat_names
{
 public:
	iocat_names();
	~iocat_names();
	string & get_name(IOCAT);
 private:
	 string* ioname;
};

#endif

// "My painting is based on the fact that only what can be seen there
// is there.  It really is an object.  Any painting is an object and
// anyone who gets involved enough in this finally has to face up to 
// the 'object-ness' of whatever it is that he's doing." - Frank Stella
//

#endif
