#ifndef CHECKVALUE_H
#include "../include/checkvalue.h"

bool check_value(long double n, stockerrs::ERR_TYPE et)
{
    bool retv = false;
    switch(et) {
        case stockerrs::ERR_RANGE_0_1:
            if (n >= 0 && n <= 1)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_M1_1:
            if (n >= -1 && n <= 1)
                retv = true;
            break; 
        case stockerrs::ERR_RANGE_CLIP:
            if (n >= 1 && n <= 32767)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_AMP:
            if (n >= -32767 && n <= 32767)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_FEED:
            if (n >= - MAX_FEEDBACK && n <= MAX_FEEDBACK)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_FMOD:
            if (n > 0 && n <= MAX_FMOD_SIZE)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_DEGS:
            if (n >= 0 && n <= 360)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_SEMI:
            if (n >= -12 && n <= 12)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_OCT:
            if (n >= -24 && n <= 8)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_BPM:
            if (n >= 10 && n <= 1000)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_BEAT:
            if (n >= 2 && n <= 32)
                retv = true;
            break;

        case stockerrs::ERR_RANGE_0_1_IN:
            if (n > 0 && n < 1)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_M1_1_IN:
            if (n > -1 && n < 1)
                retv = true;
            break;
        case stockerrs::ERR_RANGE_FREQ:
            if (n > 0 && n < audio_samplerate / MAX_FREQ_SR_DIV)
                retv = true;
            break;

        case stockerrs::ERR_NEGATIVE:
            if (n >= 0)
                retv = true;
            break;
        case stockerrs::ERR_NEG_ZERO:
            if (n > 0)
                retv = true;
            break;
        case stockerrs::ERR_ABOVE1:
            if (n > 1)
                retv = true;
            break;

        default:
            break;
    }
    return retv;
}

#endif
