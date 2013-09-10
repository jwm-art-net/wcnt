#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <math.h>
#include <stdlib.h>
#include "synthmodule.h"

#define NOTE_NAME_SIZE 5

unsigned long convert_freq_to_samples(double f);
unsigned long convert_notelen_to_samples(short nl);
unsigned long convert_ms_to_samples(double t);
double notelength_to_frequency(short nl);
double convert_notelength_to_ms(short nl);
double note_to_step(const char *note_name, char oct_offset);
double freq_to_step(double freq, char oct_offset);
double freq_to_step(double freq, char oct_offset, double semitones);
double note_to_freq(const char *note_name, char oct_offset);

#endif /* CONVERSIONS_H */
