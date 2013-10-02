#include "../include/stockerrs.h"
#include <iostream>

namespace errors
{
 const char* stock::major =   "*** MAJOR ERROR ***";
 const char* stock::bad =     "Bad attempt made to";
 const char* stock::bad_add = "Bad attempt made to add invalid object type";

 const struct stock::gn_data stock::data[LAST_TYPE] =
 {
    { ERR_TYPE,         "BAD_ERROR_TYPE",   CAT_ERR_TYPE,               "Erroneous error!"  },
    { NO_ERROR,         "Ok",               CAT_NO_ERROR,               "No error occurred." },
    // CAT_NUM_OUT_OF_RANGE: "exceeds range..."
    { RANGE_0_1,        "0.0 ~ 1.0",        CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_M1_1,       "-1.0 ~ 1.0",       CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_CLIP,       "1 ~ 32767",        CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_AMP,        "-32767 ~ 32767",   CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_FEED,       "-2.0 ~ 2.0",       CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_DEGS,       "0.0 ~ 360.0",      CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_SEMI,       "-12.0 ~ 12.0",     CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_OCT,        "-24.0 ~ 8.0",      CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_BPM,        "-500 ~ 500",       CAT_NUM_OUT_OF_RANGE,       0   },
    { RANGE_BEAT,       "2 ~ 32",           CAT_NUM_OUT_OF_RANGE,       0   },
    // CAT_NUM_OUT_OF_RANGE_OR_EQ: "should be within range..."
    { RANGE_0_1_IN,     "0.0 ~ 1.0",            CAT_NUM_OUT_OF_RANGE_OR_EQ, 0   },
    { RANGE_M1_1_IN,    "-1.0 ~ 1.0",           CAT_NUM_OUT_OF_RANGE_OR_EQ, 0   },
    { RANGE_FMOD,       "0.0 ~ 32.0",           CAT_NUM_OUT_OF_RANGE_OR_EQ, 0   },
    { RANGE_FREQ,       "0.0 ~ 1/4 sample rate",CAT_NUM_OUT_OF_RANGE_OR_EQ, 0   },
    // CAT_NUM_BELOW: "must be... "
    { NEGATIVE,         "positive",     CAT_NUM_BELOW,  0   },
    { NEG_OR_ZERO,      "above zero",   CAT_NUM_BELOW,  0   },
    { ABOVE1,           "above 1.0",    CAT_NUM_BELOW,  0   },
    { LESS_THAN,        "more than",    CAT_NUM_BELOW,  0   },
    { ATLEAST2,         "atleast two",  CAT_NUM_BELOW,  0   },
    // CAT_COULD_NOT: "could not be..."
    { NO_OPEN,          "opened",       CAT_COULD_NOT,  0   },
    { NO_ADD,           "added",        CAT_COULD_NOT,  0   },
    // CAT_INVALID:  "invalid..."
    { ISNOT,            " ",                    CAT_INVALID,    0   },
    { NOTENAME,         "note name",            CAT_INVALID,    0   },
    { INVALID_OBJECT,   "invalid object type",  CAT_INVALID,    0   },
    { ERROR,            " ",                    CAT_OTHER,      0   },
 };

 const char* stock::prefix_msg[CAT_ERR_LAST] =
 {
    major,
    "there is no error",
    "out of range",
    "should be inside range",
    "must be",
    "could not be",
    "is not a",
    "other error"
 };
}; // namespace errors
