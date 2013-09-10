#ifndef DOBJNAMES_H
#define DOBJNAMES_H

#ifndef BARE_MODULES

#include <string>

// if you want to add new dobj types then MAKE SURE you add them 
// to the CORRECT part of the enum.... and be prepared for hassle!

// DOBJ_TYPE check_type(DOBJ_TYPE)
// checks the type passed is a usable type, ie is not one of these types:
// 		DOBJ_FIRST DOBJ_NONE DOBJ_INLISTS DOBJ_INSINGLES 
//		DOBJ_DEFLISTS DOBJ_DEFSINGLES DOBJ_LAST
// if it's one of them, it returns DOBJ_FIRST, otherwise, if it's a 
// usable type it will return the value passed.

class dobjnames
{
 public:
	 enum DOBJ_TYPE
 	{
		//---------------------------------------------
		DOBJ_FIRST = 0,	// keep first & error type
		//---------------------------------------------
		DOBJ_NONE,		// hmmm, no dobj inserted?
		//---------------------------------------------
		DOBJ_INLISTS,	// seperator
		//---------------------------------------------
		// lists to be defined within the module
		// only used in moddobj - no dobj created
		LIN_ENVELOPE,	// contains SIN_COORD
		LIN_WAVEFORM,	// contains SIN_VERTEX
		LIN_METER,		// contains SIN_METER
		LIN_BPM,		// contains SIN_BPM
		LIN_TRACK, 		// contains SIN_RIFFNODE
		LIN_MIX,		// contains SMOD_CHANNEL
		LIN_SIGNALS,	// contains SMOD_SIGNAL
		LIN_DYNAMICS,	// contains SIN_DVERTEX
		//---------------------------------------------
		DOBJ_INSINGLES,	// seperator
		//---------------------------------------------
		// single objects to be inserted in lists
		// dobj inherited
		SIN_NOTE,		// dobj
		SIN_COORD,		// dobj
		SIN_VERTEX,		// dobj
		SIN_METER,		// dobj
		SIN_BPM,		// dobj
		SIN_RIFFNODE,	// dobj
		SIN_DVERTEX,
		//---------------------------------------------
		DOBJ_DEFLISTS,	// seperator.  
		// *** none within module defined here after **
		//---------------------------------------------
		// lists to be defined outside the module, 
		// and inserted by name
		// ie the list is a stand alone dobj
		LDEF_RIFF,		// contains SIN_NOTE's
		LDEF_SAMPLESET,	// contains SDEF_SAMPLEDATA's
		LDEF_RULESET,	// contains SDEF_MAPRULE's
		//---------------------------------------------
		DOBJ_DEFSINGLES,// seperator
		//---------------------------------------------
		// single stand alone dobj's defined outside of
		// a module or list, and inserted by name, as 
		// a parameter into either dobj or module. ??
		SDEF_WAVFILEIN,
		SDEF_SAMPLEDATA,
		SDEF_MAPRULE,
		//---------------------------------------------
		// special dobj for inserting synthmodules into 
		// list unbeknown to user. see dobjmod.h for 
		// more info.
		DOBJ_SYNTHMOD,
		//---------------------------------------------
		DOBJ_LAST		// keep last
		//---------------------------------------------
	};
	dobjnames();
	~dobjnames();
	string & get_name(DOBJ_TYPE);
	DOBJ_TYPE get_type(string const*);
	DOBJ_TYPE check_type(DOBJ_TYPE);
	DOBJ_TYPE get_sub_type(DOBJ_TYPE);
	string & get_sub_name(DOBJ_TYPE);
 private:
	 string* dobj_name;
};
#endif
#endif
