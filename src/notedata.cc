#ifndef NOTEDATA_H
#include "../include/notedata.h"

note_data::note_data(const char *name, double len, double pos, double vel)
    :
length(len),
position(pos),
velocity(vel),
frequency(0)
{
    set_name(name);
}

note_data::~note_data()
{
}

void
note_data::set_name(const char *name)
{
    strncpy(notename, name, NOTE_NAME_SIZE - 1);
    notename[NOTE_NAME_SIZE - 1] = '\0';        /* incase len > 4 chars */
    frequency = note_to_freq(notename, 0);
    if (frequency == 0.00) {    /* invalid note name - blank out */
        for (char i = 0; i < NOTE_NAME_SIZE; i++)
            notename[i] = '\0';
    }
}

char
checknotename(const char *n)
{
    if (n == 0)
        return -5;
    int charcount = strlen(n);
    if (n[0] < 'a' || n[0] > 'g')
        return -1;
    if (charcount < 2 || charcount > 4)
        return -2;
    if (charcount == 2 && (n[1] < '0' || n[1] > '9'))
        return -3;
    if (charcount == 3 && (n[2] < '0' || n[2] > '9'))
        return -3;
    if (charcount == 4 && (n[3] < '0' || n[3] > '9'))
        return -3;
    if (charcount == 3 && (n[1] != '#' && n[1] != '-'))
        return -4;
    if (charcount == 4 && (n[1] != '#' || n[2] != '-'))
        return -4;
    return 0;
}

#endif
