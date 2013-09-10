#ifndef BPMCHANGE_H
#include "../include/bpmchange.h"

bpmchange & bpmchange::operator=(const bpmchange & rhs)
{
	if (this == &rhs) return *this;
	atbar = rhs.atbar;
	tobpm = rhs.tobpm;
	return *this;
}

#endif
