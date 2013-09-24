#include "../include/iocat.h"


#ifdef DEBUG
#include <iostream>
#endif

iocat::iocat()
{
#ifdef DEBUG
    for (int i = FIRST; i < LAST; i++){
        if (data[i].cat != i) {
            std::cout << "\n***** iocat error *****\n";
            std::cout << data[i].name << " is in array index " << i;
            std::cout << " but has type index of " << data[i].cat;
            std::cout << "\nthese values should tally!\n";
        }
    }
#endif
}

#ifdef DEBUG
static iocat iocatnames;
#endif


const char* iocat::get_name(IOCAT iocat)
{
    if (iocat <= FIRST || iocat >= LAST)
        return data[FIRST].name;
    return data[iocat].name;
}

const iocat::iocat_data iocat::data[LAST] =
{
    { FIRST,    "Error Cat"                     },
    { DOUBLE,   "float_value"                   },
    { SHORT,    "integer_value"                 },
    { SAMP_T,   "integer_value"                 },
    { TRIG,     "on/off"                        },
    { STATE,    "on/off"                        },
    { STRING,   "text_string"                   },
    { FIX_STR,  "fixed_string"                  },
    { METER,    "integer_value/integer_value"   },
    { DOBJ,     "data_object_name"              },
    { SYNTHMOD, "module_name"                   }
};
