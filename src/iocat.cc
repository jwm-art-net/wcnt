#include "../include/iocat.h"

namespace iocat
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,     "Error Cat",                    0, "Error Category!" },
    { DOUBLE,       "float_value",                  0, "Floating point value, ie 1.375." },
    { SHORT,        "integer_value",                0, "Whole number, ie 124." },
    { ULONG,        "integer_value",                0, "Whole number, ie 124." },
    { TRIG,         "on/off",                       0, "on or off." },
    { STATE,        "on/off",                       0, "on or off." },
    { STRING,       "text_string",                  0, "A sequence of characters not containing spaces." },
    { FIX_STR,      "fixed_string",                 0, "Multiple choice." },
    { METER,        "integer_value/integer_value",  0, "Time signature, ie 5/8." },
    { DOBJ,         "data_object_name",             0, "The name of a data object." },
    { SYNTHMOD,     "module_name",                  0, "The name of a module." }
 };

}; // namespace iocat
