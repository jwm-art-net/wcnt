#include "../include/checkvalue.h"
#include "../include/jwm_globals.h"
#include "../include/jwm_ranges.h"

#include <iostream>

bool check_value(long double n, errors::TYPE et)
{
    switch(et)
    {
    case errors::RANGE_0_1:
        if (n >= 0 && n <= 1)
            return true;
        break;
    case errors::RANGE_M1_1:
        if (n >= -1 && n <= 1)
            return true;
        break;
    case errors::RANGE_FEED:
        if (n >= -jwm_ranges::max_feedback_level
         && n <=  jwm_ranges::max_feedback_level)
            return true;
        break;
    case errors::RANGE_FMOD:
        if (n > 0 && n <= jwm_ranges::max_fmod_size)
            return true;
        break;
    case errors::RANGE_DEGS:
        if (n >= 0 && n <= jwm_ranges::max_degs)
            return true;
        break;
    case errors::RANGE_SEMI:
        if (n >= -jwm_ranges::max_semitone
         && n <=  jwm_ranges::max_semitone)
            return true;
        break;
    case errors::RANGE_SEMI2:
        if (n >= -jwm_ranges::max_semitone * 2
         && n <=  jwm_ranges::max_semitone * 2)
            return true;
        break;
    case errors::RANGE_OCT:
        if (n >= jwm_ranges::min_oct
         && n <= jwm_ranges::max_oct)
            return true;
        break;
    case errors::RANGE_BPM:
        if (n >= -jwm_ranges::max_bpm_offset
         && n <=  jwm_ranges::max_bpm_offset)
            return true;
        break;
    case errors::RANGE_BEAT:
        if (n >= 2 && n <= 32)
            return true;
        break;
    case errors::RANGE_COUNT:
        if (n >= 0 && n <= WCINT_T_MAX)
            return true;
        break;
    case errors::RANGE_COUNT1:
        if (n >= 1 && n <= WCINT_T_MAX)
            return true;
        break;
     case errors::RANGE_SAMPLE:
        if (n >= 0 && n <= SAMP_T_MAX)
            return true;
        break;
   case errors::RANGE_0_1_IN:
        if (n > 0 && n < 1)
            return true;
        break;
    case errors::RANGE_M1_1_IN:
        if (n > -1 && n < 1)
            return true;
        break;
    case errors::RANGE_FREQ:
        if (n > 0 && n < jwm.samplerate() * jwm_ranges::max_freq_sr_ratio)
            return true;
        break;
    case errors::NEGATIVE:
        if (n >= 0)
            return true;
        break;
    case errors::NEG_OR_ZERO:
        if (n > 0)
            return true;
        break;
    case errors::ABOVE1:
        if (n > 1)
            return true;
        break;
    default:
        std::cout << "***** BADNESS: Unhandled error range :-/ *****" << std::endl;
        return false;
    }
    return false;
}

