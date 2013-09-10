#ifndef WAVHEADER_H
#include "../include/wavfileheader.h"

wavheader::wavheader(
 short channels, unsigned long samplerate, short resolution) :
 itsLength(36), its_length(16), format_tag(1), itsChannels(channels),
 itsSampleRate(samplerate), itsResolution(resolution), itsSampleLength(0)
{
    itsText1[0] = 'R';
    itsText1[1] = 'I';
    itsText1[2] = 'F';
    itsText1[3] = 'F';
    // itsLength
    itsText2[0] = 'W';
    itsText2[1] = 'A';
    itsText2[2] = 'V';
    itsText2[3] = 'E';
    itsData1[0] = 'f';
    itsData1[1] = 'm';
    itsData1[2] = 't';
    itsData1[3] = ' ';
    //its_length
    //format_tag
    //itsChannels
    //itsSampleRate
    itsBlockSampleRate = itsBlockAlign * itsSampleRate;
    itsBlockAlign = (itsResolution / 8) * itsChannels;
    //itsResolution
    itsText3[0] = 'd';
    itsText3[1] = 'a';
    itsText3[2] = 't';
    itsText3[3] = 'a';
    //itsSampleLength
}

bool wavheader::valid_format_name()
{
    char* fmt = new char[5];
    strncpy(fmt, itsText1, 4);
    fmt[4] = '\0';
    if (strcmp(fmt, "RIFF") == 0) {
        delete [] fmt;
        return true;
    }
    delete [] fmt;
    return false;
}

bool wavheader::valid_type_name()
{
    char* tn = new char[5];
    strncpy(tn, itsText2, 4);
    tn[4] = '\0';
    if (strcmp(tn, "WAVE") == 0) {
        delete [] tn;
        return true;
    }
    delete [] tn;
    return false;
}

bool wavheader::valid_riff_name()
{
    char* rn = new char[5];
    strncpy(rn, itsData1, 4);
    rn[4] = '\0';
    if (strcmp(rn, "fmt ") == 0) {
        delete [] rn;
        return true;
    }
    delete [] rn;
    return false;
}

WAV_WRITEHEAD wavheader::SetReady(unsigned long &length)
{
    itsSampleLength = length * itsBlockAlign;
    //	itsLength = itsSampleLength + sizeof(wavheader);
    //	its_length = itsSampleLength;
    if (itsChannels < 1 || itsChannels > 2)
        return WAV_WH_CHERR;
    if (itsResolution % 8 != 0)
        return WAV_WH_RESERR;
    return WAV_WH_OK;
}

#endif
