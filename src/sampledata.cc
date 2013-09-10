#ifndef SAMPLEDATA_H
#include "../include/sampledata.h"

sample_data::sample_data() :
        dobj(dobjnames::SDEF_SAMPLEDATA)

        /*
        :
        	play_dir(PLAY_FORWARD),
        	loop1_mode(LOOP_OFF),
        	loop2_mode(LOOP_OFF),
        	note_off_mode(NOFF_TO_END),
        	start_sample(0),
        	end_sample(0),
        	loop1_start_sample(0),
        	loop1_end_sample(0),
        	loop2_start_sample(0),
        	loop2_end_sample(0)
        	*/
{
    // just like the good old days!
}


stockerrs::ERR_TYPE sample_data::validate()
{
    *err_msg = "sampledata is not yet implimented";
    return stockerrs::ERR_ERROR;
}

bool sample_data::set_dparam(dparamnames::DPAR_TYPE, void*)
{
    return false;
}

void* sample_data::get_dparam(dparamnames::DPAR_TYPE pt)
{
    return 0;
}

#ifndef BARE_MODULES
bool sample_data::done_dparams = false;

void sample_data::create_dparams()
{
    return;
}
#endif

#endif
