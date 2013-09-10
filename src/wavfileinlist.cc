#ifndef WAVFILEINLIST_H
#include "../include/wavfileinlist.h"

#ifndef BARE_MODULES

wavfilein_list::wavfilein_list()
: wavfilelist(0), wavfile_item(0), wavfile(0)
{
	wavfilelist = new 
		linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

wavfilein_list::~wavfilein_list()
{
	ll_item* tmp = wavfilelist->goto_first();
	while(tmp)
	{
		delete (wavfilein*)tmp->get_data();
		tmp = wavfilelist->goto_next();
	}
}

wavfilein* wavfilein_list::add_wavfile(wavfilein* rd)
{
	if (!rd)
		return 0;
	return wavfile = (wavfilein*)(wavfile_item = wavfilelist->add_at_head(rd))->get_data();
}

bool wavfilein_list::delete_wavfile(wavfilein* rd)
{
	if (!rd)
		return false;
	ll_item* tmp = wavfilelist->find_data(rd);
	if (!tmp)
		return false;
	if (tmp == wavfile_item)
		goto_first();
	delete wavfilelist->unlink_item(tmp);
	delete rd;
	return true;
}

wavfilein* wavfilein_list::get_wavfilein_by_name(string const* wavname)
{
	goto_first();
	while(wavfile)
	{
		if (*wavname == wavfile->get_sample_name())
			return wavfile;
		goto_next();
	}
	return 0;
}

#endif
#endif
