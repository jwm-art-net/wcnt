#ifndef CHECKVALUE_H
#define CHECKVALUE_H

#include "stockerrs.h"

// need synthmod for check of frequency 
// against samplerate:

#include "synthmodule.h"

bool check_value(long double n, stockerrs::ERR_TYPE et);

#endif
