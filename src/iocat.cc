#include "../include/iocat.h"

namespace iocat
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,     "Error Cat",            0, "Error Category!" ,0 },
    { DOUBLE,       "DECIMAL",              0, "Floating point value, ie 1.375." ,0 },
    { WCINT_T,      "INTEGER",              0, "Whole number, ie 12431." ,0 },
    { SAMP_T,       "INTEGER",              0, "Whole number, ie 124222211241." ,0 },
    { TRIG,         "on|off",               0, "on or off." ,0 },
    { STATE,        "on|off",               0, "on or off." ,0 },
    { STRING,       "TEXT",                 0, "A sequence of characters not containing spaces." ,0 },
    { FIX_STR,      "TEXT",                 0, "Multiple choice." ,0 },
    { METER,        "INTEGER/INTEGER",      0, "Time signature, ie 5/8." ,0 },
    { DOBJ,         "DATA-OBJECT",          0, "The name of a data object." ,0 },
    { SYNTHMOD,     "MODULE",               0, "The name of a module." ,0 },
    { DOUBLE1,      "DECIMAL",              0, "Internal usage only." ,0 }
 };
}; // namespace iocat
