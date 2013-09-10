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
   length value:   48  24    12     6      3         1.5
   these values apply UNLESS you have changed #define QUARTER_VALUE
   in timemap.h from 12 to some other value.
  ------------------------------------------------------------------*/
/*
  whole half quarter eighth sixteenth thirtysecond  sixtyfourth  128     256
   768  384   192      96      48           24          12        6      3  
   384  192    96      48      24           12           6        3      1.5
   192   96    48      24      12            6           3        1.5    0.75
*/

// quarter_value will be defined in each riff to allow the user to use
// note position and length suitable for pattern type. ie
// quarter_value of 4 when the pattern contains no notes or positions
// requiring accuracy less than sixteenth etc 

char checknotename(const char* n);

class note_data 
{
 public:
	note_data(const char* name, double len, double pos, double vel);
	~note_data();
	void set_name(const char * n);
	void set_length(double l) { length = l; }
	void set_position(double p) { position = p; }
	void set_velocity(double v) { velocity = v; }
	void set_frequency(double f) { frequency = f; }
	double get_length() { return (this == NULL) ? 0 : length; }
	double get_position() { return (this == NULL) ? 0 : position; }
	double get_velocity() { return (this == NULL) ? 0.00 : velocity; }
	const char* get_name() { return (this == NULL) ? NULL : notename; }
	double get_frequency() { return (this == NULL) ? 0.00 : frequency; }
 private:
	char notename[NOTE_NAME_SIZE];
	double length;
	double position;
	double velocity;
	double frequency;
};

#endif
