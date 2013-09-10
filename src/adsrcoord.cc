#ifndef ADSRCOORD_H
#include "../include/adsrcoord.h"

adsr_coord::adsr_coord(adsr_coord::SECT s, double ut, double ul, double lt, double ll)
: sect(s), upper_time(ut), upper_level(ul), lower_time(lt), lower_level(ll)
{
}

adsr_coord::~adsr_coord()
{
}

void adsr_coord::run(double velocity) 
{
	if (velocity < 0) 
		velocity = -velocity;
	output_time = lower_time + (upper_time - lower_time) * velocity;
	output_level = lower_level + (upper_level - lower_level) * velocity;
}

#endif
