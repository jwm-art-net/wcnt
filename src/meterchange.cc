#ifndef METERCHANGE_H
#include "../include/meterchange.h"

meterchange & meterchange::operator=(const meterchange & rhs)
{
	if (this == &rhs) return *this;
	bar = rhs.bar;
	beatperbar = rhs.beatperbar;
	beatvalue = rhs.beatvalue;
	return *this;
}

#endif
