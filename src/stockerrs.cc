#include "../include/stockerrs.h"
#include <iostream>

const char* stockerrs::major =      "*** MAJOR ERROR ***";
const char* stockerrs::bad =        "Bad attempt made to";
const char* stockerrs::bad_add =    "Bad attempt made to add "
                                    "invalid object type";

stockerrs::stockerrs()
{
    #ifdef DEBUG
    for (int i = ERR_FIRST; i < ERR_LAST; ++i) {
        if (data[i].type != i) {
            std::cout << "\n***** stockerrs error *****\n";
            std::cout << data[i].msg << " is in array index " << i;
            std::cout << " but has type index of " << data[i].type;
            std::cout << "\nthese values should tally!\n";
        }
    }
    #endif
}

#ifdef DEBUG
static stockerrs stockerrors;
#endif

stockerrs::~stockerrs()
{
}

char const* stockerrs::get_err(stockerrs::ERR_TYPE et)
{
    if (et > ERR_FIRST && et < ERR_LAST)
        return data[et].msg;
    else
        return data[ERR_FIRST].msg;
}

char const* stockerrs::get_prefix_err(stockerrs::ERR_TYPE et)
{
    if (et > ERR_RANGE_TYPE1 && et < ERR_RANGE_TYPE2)
        return prefixmsg[1];
    if (et > ERR_RANGE_TYPE2 && et < ERR_TYPE3)
        return prefixmsg[2];
    if (et >= ERR_NEG_ZERO && et <= ERR_ATLEAST2)
        return prefixmsg[3];
    if (et >= ERR_NO_OPEN && et <= ERR_NO_ADD)
        return prefixmsg[4];
    if (et >= ERR_ISNOT && et <= ERR_NOTENAME)
        return prefixmsg[5];
    return prefixmsg[0];
}


bool stockerrs::check_type(ERR_TYPE et)
{
    switch (et)
    {
        case ERR_FIRST:
        case ERR_RANGE_TYPE1:
        case ERR_RANGE_TYPE2:
        case ERR_TYPE3:
        case ERR_TYPE4:
        case ERR_LAST:
            return false;
        default:
            return true;
    }
}

const stockerrs::err_msg_data stockerrs::data[ERR_LAST] =
{
    { ERR_FIRST,        major           },
    { ERR_NO_ERROR,     "ok"            },

    { ERR_RANGE_TYPE1,  major           },
    { ERR_RANGE_0_1,    "0.0 ~ 1.0"     },
    { ERR_RANGE_M1_1,   "-1.0 ~ 1.0"    },
    { ERR_RANGE_CLIP,   "1 ~ 32767"     },
    { ERR_RANGE_AMP,    "-32767 ~ 32767"},
    { ERR_RANGE_FEED,   "-2.0 ~ 2.0"    },
    { ERR_RANGE_DEGS,   "0.0 ~ 360.0"   },
    { ERR_RANGE_SEMI,   "-12.0 ~ 12.0"  },
    { ERR_RANGE_OCT,    "-24.0 ~ 8.0"   },
    { ERR_RANGE_BPM,    "-500 ~ 500"    },
    { ERR_RANGE_BEAT,   "2 ~ 32"        },

    { ERR_RANGE_TYPE2,  major           },
    { ERR_RANGE_0_1_IN, "0.0 ~ 1.0"     },
    { ERR_RANGE_M1_1_IN,"-1.0 ~ 1.0"    },
    { ERR_RANGE_FMOD,   "0.0 ~ 32.0"    },
    { ERR_RANGE_FREQ,   "0.0 ~ 1/4 sample rate"},

    { ERR_TYPE3,        major           },
    { ERR_NEGATIVE,     "is negative"   },// must not be
    { ERR_NEG_ZERO,     "above zero"    }, // must be
    { ERR_ABOVE1,       "above 1.0"     },    //  "   "

    { ERR_TYPE4,        major           },
    { ERR_LESS_THAN,    "more than"     }, // must be
    { ERR_ATLEAST2,     "atleast two"   },//  "   "
    { ERR_NO_OPEN,      "opened"        }, // could not be
    { ERR_NO_ADD,       "added"         },   //   "    "  "
    { ERR_ISNOT,        " "             },
    { ERR_NOTENAME,     "note name"     },
    { ERR_INVALID_OBJECT,   "invalid object type"},
    { ERR_ERROR,        " "},
};


const char* stockerrs::prefixmsg[] = {  " ",
                                        " out of range ",
                                        " should be inside range ",
                                        " must be ",
                                        " could not be ",
                                        " is not a "        };
