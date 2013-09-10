#ifndef NOTEDATA_H
#define NOTEDATA_H

#include <string.h>
#include "synthmodule.h"
#include "linkedlist.h"
#include "conversions.h"


#define NOTE_NAME_SIZE 5

/*-----------------NOTE LENGTH & NOTE LENGTH VALUES-----------------
  ------------------------------------------------------------------
   note_length :  whole half quarter eigth sixteenth thirtysecond
   length value:   256  128    64     32      16         8
  ------------------------------------------------------------------*/

char checknotename(const char* n);

class note_data 
{
 public:
	note_data(const char* name, short len, short pos, double vel);
	~note_data();
	void set_name(const char * n);
	void set_length(short l) { length = l; }
	void set_position(short p) { position = p; }
	void set_velocity(double v) { velocity = v; }
	void set_frequency(double f) { frequency = f; }
	short get_length() { return (this == NULL) ? 0 : length; }
	short get_position() { return (this == NULL) ? 0 : position; }
	double get_velocity() { return (this == NULL) ? 0.00 : velocity; }
	const char* get_name() { return (this == NULL) ? NULL : notename; }
	double get_frequency() { return (this == NULL) ? 0.00 : frequency; }
 private:
	char notename[NOTE_NAME_SIZE];
	short length;
	short position;
	double velocity;
	double frequency;
};

#endif
