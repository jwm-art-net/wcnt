#ifndef CHECKVALUE_H
#define CHECKVALUE_H

#include "stockerrs.h"


/* checks numerical values...

   ...like so:

        if (!check_value((long double)my_int_val, errors::NEGATIVE))
            report_error(...);
*/

bool check_value(long double n, errors::TYPE et);

#endif
