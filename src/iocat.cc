#include "../include/iocat.h"

namespace iocat
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,     "Error Cat",            0, "Error Category!" },
    { DOUBLE,       "DECIMAL",              0, "Floating point value, ie 1.375." },
    { WCINT_T,      "INTEGER",              0, "Whole number, ie 12431." },
    { SAMP_T,       "INTEGER",              0, "Whole number, ie 124222211241." },
    { TRIG,         "on|off",               0, "on or off." },
    { STATE,        "on|off",               0, "on or off." },
    { STRING,       "TEXT",                 0, "A sequence of characters not containing spaces." },
    { FIX_STR,      "TEXT",                 0, "Multiple choice." },
    { METER,        "INTEGER/INTEGER",      0, "Time signature, ie 5/8." },
    { DOBJ,         "DATA-OBJECT",          0, "The name of a data object." },
    { SYNTHMOD,     "MODULE",               0, "The name of a module." },
    { SPECIAL,      "SPECIAL",              0, "It is special." }
 };
}; // namespace iocat
