#include "../include/conversions.h"
#include "../include/globals.h"

#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>

double note_to_step(const char *note_name)
{
    return freq_to_step(note_to_freq(note_name));
}

double note_to_step(const char *note_name, char oct_offset)
{
    return freq_to_step(note_to_freq(note_name, oct_offset));
}

double note_to_step(
 const char *note_name, char oct_offset, double semitones)
{
    return freq_to_step(note_to_freq(note_name, oct_offset, semitones));
}

double freq_to_step(double freq)
{
    if (freq == 0) return 0;
    return 360.0 / ((double)wcnt::jwm.samplerate() / freq);
}

double freq_to_step(double freq, char oct_offset)
{
    if (freq == 0) return 0;
    double fr = 1;
    if (oct_offset > 0)
        fr = 2 * oct_offset;
    else if (oct_offset < 0)
        fr = (double)1 / (2 * -oct_offset);
    return 360.0 / ((double)wcnt::jwm.samplerate() / (freq * fr));
}

double freq_to_step(double freq, char oct_offset, double semitones)
{
    if (freq == 0) return 0;
    double fr = 1;
    if (oct_offset > 0)
        fr = 2 * oct_offset;
    else if (oct_offset < 0)
        fr = (double)1 / (2 * -oct_offset);
    return 360 / ((double)wcnt::jwm.samplerate() /
                  (freq * fr * pow(2, (semitones / 12))));
}

double note_to_freq(const char* note_name)
{
    if (!check_notename(note_name)) return 0;
    char note_no = note_to_noteno(note_name);
    char oct = extract_octave(note_name);
    return 440 * pow(2, oct + (double)(note_no - 10) / 12);
}

double note_to_freq(const char *note_name, char oct_offset)
{
    if (!check_notename(note_name)) return 0;
    char note_no = note_to_noteno(note_name);
    char oct = extract_octave(note_name);
    return 440 * pow(2, (oct + oct_offset) + (double)(note_no - 10) / 12);

}

double note_to_freq(
 const char* note_name, char oct_offset, double semitones)
{
    if (!check_notename(note_name)) return 0;
    char note_no = note_to_noteno(note_name);
    char oct = extract_octave(note_name);
    double freq =
        440 * pow(2, (oct + oct_offset) + (double)(note_no - 10) / 12);
    return freq * pow(2, (semitones / 12));
}

const char* transpose_notename(const char* note_name, char semitones)
{
    if (!check_notename(note_name)) return 0;
    char sub_oct = 0;
    if (semitones < 0) {
        sub_oct = semitones / 12 - 1;
        semitones -= (sub_oct ) * 12;
    }
    char note_no = note_to_noteno(note_name) + semitones - 1;
    char oct = extract_octave(note_name) + sub_oct + note_no / 12;
    if (oct < -9) oct = -9;
    else if (oct > 9) oct = 9;
    int scalepos = (note_no % 12) * 2 + 1;
    const char* scale = " c c#d d#e f f#g g#a a#b";
    char* newname = new char[3];
    newname[0] = scale[scalepos];
    if (scale[scalepos + 1] == '#') {
        newname[1] = '#';
        newname[2] = '\0';
    }
    else newname[1] = '\0';
    char* newnotename = new char[wcnt::note_array_size];
    sprintf(newnotename, "%s%d", newname, oct);
    delete [] newname;
    return newnotename;
}

bool check_notename(const char *n)
{
    if (!n)
        return false;
    int len = strlen(n);
    if (n[0] < 'a' || n[0] > 'g')
        return false;
    if (len < 2 || len > 4)
        return false;
    if ((n[0] == 'b' || n[0] == 'e') && n[1] == '#')
        return false;
    if (len == 2 && (n[1] < '0' || n[1] > '9'))
        return false;
    if (len == 3 && (n[2] < '0' || n[2] > '9'))
        return false;
    if (len == 4 && (n[3] < '0' || n[3] > '9'))
        return false;
    if (len == 3 && (n[1] != '#' && n[1] != '-'))
        return false;
    if (len == 4 && (n[1] != '#' || n[2] != '-'))
        return false;
    return true;
}

char note_to_noteno(const char* note_name)
{// does not check notename because it's done before this is called
    const char* scale = " c c#d d#e f f#g g#a a#b";
    char name[3];
    name[0] = note_name[0];
    if (note_name[1] == '#') {
        name[1] = '#';
        name[2] = '\0';
    }
    else name[1] = '\0';
    return (1 + strstr(scale, name) - scale) / 2.0f;
}

char extract_octave(const char* note_name)
{// does not check notename because it's done before this is called
    if (note_name[1] == '#')
        return atoi(&note_name[2]);
    else
        return atoi(&note_name[1]);
}

samp_t freq_to_samples(double f)
{
    return (samp_t)(wcnt::jwm.samplerate() / f);
}

samp_t ms_to_samples(double t)
{
    return (samp_t)(wcnt::jwm.samplerate() * (t / 1000.0f));
}

