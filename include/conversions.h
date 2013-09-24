#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include "types.h"

/*----------------------------------------------------------------
    conversion of note names to frequencies have been adapted 
    from code contained in a book by Ian Waugh, entitled:
    Making Music on The Amstrad CPC 464 & 664.  with additions
    for tuning frequencies via semitones from sources on the web
    which I cannot now recall, sorry.
----------------------------------------------------------------*/

// note name conversions - with/without transpose
// (transpose_notename is not used by them)
double note_to_step(const char *note_name);
double note_to_step(const char *note_name, char oct_offset);
double note_to_step(
    const char *note_name, char oct_offset, double semitones);
double freq_to_step(double freq);
double freq_to_step(double freq, char oct_offset);
double freq_to_step(double freq, char oct_offset, double semitones);
double note_to_freq(const char *note_name);
double note_to_freq(const char *note_name, char oct_offset);
double note_to_freq(
    const char *note_name, char oct_offset, double semitones);

// transpose one note name to another note name
const char* transpose_notename(const char* note_name, char semitones);

// helper functions used by note_to_???? functions
// (check_notename no longer accepts e# and b#)
bool check_notename(const char *n);
// note name should be checked before calling these two
// note that note_to_noteno does not consider octaves.
char note_to_noteno(const char* note_name);
char extract_octave(const char* note_name);

// miscellaneous conversions
samp_t freq_to_samples(double f);
samp_t ms_to_samples(double t);

#endif
