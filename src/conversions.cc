#ifndef CONVERSIONS_H
#include "../include/conversions.h"

unsigned long convert_freq_to_samples(double f) 
{
	return (unsigned long)(audio_samplerate / f);
}

double freq_to_step(double freq, char oct_offset) 
{
	double fr = 1;
	if (oct_offset > 0) 
		fr = 2 * oct_offset;
	else if (oct_offset < 0) 
		fr = (double)1 / (-2 * oct_offset);
	return (360 / ((double)audio_samplerate / (freq * fr)));
}

double freq_to_step(double freq, char oct_offset, double semitones)
{
	double fr = 1;
	if (oct_offset > 0) 
		fr = 2 * oct_offset;
	else if (oct_offset < 0) 
		fr = (double)1 / (-2 * oct_offset);
	return 360 / ((double)audio_samplerate / (freq * fr * pow(2, (semitones / 12))));
}

double notelength_to_frequency(short nl)
{
	return (1000 / convert_notelength_to_ms(nl));
}

unsigned long convert_notelen_to_samples(short nl)
{
	double t = convert_notelength_to_ms(nl);
	return (convert_ms_to_samples(t));
}

unsigned long convert_ms_to_samples(double t) 
{
	return (unsigned long)(audio_samplerate * (t / 1000));
}

double convert_notelength_to_ms(short nl) 
{
	return ((double)60 / beats_per_minute) * ((double)nl / 64) * 1000;
}
double note_to_step(const char *note_name, char oct_offset) 
{
/* adapted from Ian Waugh's Book : Making Music on The Amstrad CPC 464 */
	if (note_name == NULL) 
		return 0.00;
	char	*scale = " c c#d d#e f f#g g#a a#b";
	char	name[NOTE_NAME_SIZE];
	char	len;
	char	oct;
	char	note_no;
	double	freq, step;
	
	len = strlen(note_name);
	switch (len) 
	{
		case 2:
			strncpy(name, note_name, 1);
			name[1] = 0;
			oct = atoi(&note_name[1]);
			break;
		case 3:
			if (note_name[1] == '-') {
				strncpy(name, note_name, 1);
				name[1] = 0;
				oct = atoi(&note_name[1]);
			} else {
				strncpy(name, note_name, 2);
				name[2] = 0;
				oct = atoi(&note_name[2]);
			}
			break;
		case 4:
			strncpy(name, note_name, 2);
			name[2] = 0;
			oct = atoi(&note_name[2]);
			break;
		default:
			return 0.00;
	}
	note_no = (1 + strstr(scale, name) - scale) / 2;
	freq = 440 * pow(2, (oct + oct_offset) + (double)(note_no - 10) / 12);
	step = ((double)360 / audio_samplerate) * freq;
	return(step);
}
double note_to_freq(const char *note_name, char oct_offset) 
{
/* adapted from Ian Waugh's Book : Making Music on The Amstrad CPC 464 */
	if (note_name == NULL) 
		return 0.00;
	char * scale = " c c#d d#e f f#g g#a a#b";
	char name[NOTE_NAME_SIZE];
	char len;
	int	oct;
	char note_no;
	double freq;
	len = strlen(note_name);
	switch (len) 
	{
		case 2:
			strncpy(name, note_name, 1);
			name[1] = 0;
			oct = atoi(&note_name[1]);
			break;
		case 3:
			if (note_name[1] == '-')	
			{
				strncpy(name, note_name, 1);
				name[1] = 0;
				oct = atoi(&note_name[1]);
			} 
			else 
			{
				strncpy(name, note_name, 2);
				name[2] = 0;
				oct = atoi(&note_name[2]);
			}
			break;
		case 4:
			strncpy(name, note_name, 2);
			name[2] = 0;
			oct = atoi(&note_name[2]);
			break;
		default:
			return 0.00;
	}
	note_no = (1 + strstr(scale, name) - scale) / 2;
	freq = 440 * pow(2, (oct + oct_offset) + (double)(note_no - 10) / 12);
	return(freq);
}

#endif /*CONVERSIONS_H */
