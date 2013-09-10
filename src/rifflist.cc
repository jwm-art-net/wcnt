#ifndef RIFFLIST_H
#include "../include/rifflist.h"

riff_list::riff_list()
: rifflist(0), riff_item(0), riff(0)
{
	rifflist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

riff_list::~riff_list()
{
	goto_first();
	while(riff)
	{
		delete riff;
		goto_next();
	}
}

riffdata* riff_list::add_riff(riffdata* rd)
{
	if (!rd)
		return 0;
	return riff = (riffdata*)(riff_item = rifflist->add_at_head(rd))->get_data();
}

bool riff_list::delete_riff(riffdata* rd)
{
	if (!rd)
		return false;
	ll_item* tmp = rifflist->find_data(rd);
	if (!tmp)
		return false;
	if (tmp == riff_item)
		goto_first();
	delete rifflist->unlink_item(tmp);
	delete rd;
	return true;
}

riffdata* riff_list::get_riff_by_name(string const* rname)
{
	goto_first();
	while(riff)
	{
		if (*riff->get_username() == *rname)
			return riff;
		goto_next();
	}
	return 0;
}

#endif
