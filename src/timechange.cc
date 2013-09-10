#ifndef TIMECHANGE_H
#include "../include/timechange.h"

timechange & timechange::operator=(const timechange & rhs)
{
	if (this == &rhs) return *this;
	atbar = rhs.atbar;
	tobpm = rhs.tobpm;
	beatperbar = rhs.beatperbar;
	beatvalue = rhs.beatvalue;
	return *this;
}

#endif
