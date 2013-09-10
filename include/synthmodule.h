/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef SYNTHMODULE_H
#define SYNTHMODULE_H

#include "synthmodnames.h"

// when you want to test a new module you've written without all the extra 
// data like module lists, output lists, input lists, parameter lists, and 
// connector lists:  define BARE_MODULES  - to get just that, bare modules 
// with only enough functionality for them to run(), but you'll have to hard 
// code the outputs to inputs, but that's ok when you just want to test the 
// thing.

#ifndef BARE_MODULES
#include "inputnames.h"
#include "outputnames.h"
#include "paramnames.h"
#endif
// synthmod - base class from which all synth modules are derived from
// - has recently become more useful every day.

// Default settings for audio output 
extern short audio_channels;
extern unsigned long audio_samplerate;
extern short audio_bitrate;
extern short audio_ysize;

// Default settings for music tempo 
#define MAX_BPM 2500
extern short sm_beats_per_minute;
extern short sm_beats_per_measure;
extern short sm_beat_value;

// Used by triggers and switches and states 
enum STATUS { OFF, ON };

// forward definitions
#ifndef BARE_MODULES
class synthmodlist;
class modinputlist;
class modoutputlist;
class modparamlist;
class connectorlist;
#endif

// although synthmodules do not need access to synthmodlist or connectorlist
// you can use synthmod to access them.  Anything that needs access to
// synthmodlist or connector list will need access to synthmod anyway.
// it makes life easier for passing stuff around the user interface code.

class synthmod
{
 public:
	synthmod(synthmodnames::SYNTH_MOD_TYPE, short id, string uname);
	virtual ~synthmod(){};
	synthmodnames::SYNTH_MOD_TYPE get_module_type() { return (this) ? module_type : synthmodnames::MOD_FIRST;}
	short get_number_id() { return (this) ? number_id : -1;}
	string const* get_username(){ return &username;}
	//virtual funcs
	virtual void run() = 0;
	virtual void init() = 0;
	#ifndef BARE_MODULES
	virtual void const* get_out(outputnames::OUT_TYPE) = 0;
	virtual void const* set_in(inputnames::IN_TYPE, void const*) = 0;
	virtual bool set_param(paramnames::PAR_TYPE, void const*) = 0;
	#endif
	// don't want any old scruffbag monkey making modules valid,
	// so only allow invalidate() public and keep validate() protected.
	void invalidate(){valid = false;}
	bool is_valid() { return valid;}
	#ifndef BARE_MODULES
	// some bool_arcs to do stuff around hither and tither
	static void register_modnames(synthmodnames* s) {modnames = s;}
	static void register_inputnames(inputnames* i) {innames = i;}
	static void register_outputnames(outputnames* o) {outnames = o;}
	static void register_paramnames(paramnames* p) {parnames = p;}
	static void register_modlist(synthmodlist* s){ modlist = s;}
	static void register_inputlist(modinputlist* m) {inputslist = m;}
	static void register_outputlist(modoutputlist* m) {outputslist = m;}
	static void register_paramlist(modparamlist* m){paramlist = m;}
	static void register_connectlist(connectorlist* c){ connectlist = c;}
	#endif
	// synthmodnames are best keep outside of BARE_MODULES
	static synthmodnames* get_modnames(){ return modnames;}
	#ifndef BARE_MODULES
	static inputnames* get_inputnames(){ return innames;}
	static outputnames* get_outputnames(){ return outnames;}
	static paramnames* get_paramnames(){ return parnames;}
	static synthmodlist* get_modlist(){ return modlist;}
	static modinputlist* get_inputlist(){ return inputslist;}
	static modoutputlist* get_outputlist(){ return outputslist;}
	static modparamlist* get_paramlist(){ return paramlist;}
	static connectorlist* get_connectlist(){ return connectlist;}
	#endif
 protected:
	void validate(){valid = true;}
 private:
	synthmodnames::SYNTH_MOD_TYPE module_type;
	short number_id;
	string username;
	bool valid;
	static synthmodnames* modnames;
	#ifndef BARE_MODULES
	static inputnames* innames;
	static outputnames* outnames;
	static paramnames* parnames;
	static synthmodlist* modlist;
	static modinputlist* inputslist;
	static modoutputlist* outputslist;
	static modparamlist* paramlist;
	static connectorlist* connectlist;
	#endif
};

#endif
