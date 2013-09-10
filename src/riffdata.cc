#ifndef RIFFDATA_HH
#include "../include/riffdata.h"

riffdata::riffdata(string& uname): 
	username(0), quarter_val(12), note(0), notelist(0)
{
	notelist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
	username = new string(uname);
}

riffdata::~riffdata() 
{
	ll_item* tmp = notelist->goto_first();
	while(tmp)
	{
		delete (note_data*)tmp->get_data();
		tmp = notelist->goto_next();
	}
	delete notelist;
	delete username;
}

note_data* riffdata::insert_and_position_note(note_data* newnote)
{
	if (!newnote) 
		return 0;
	if (checknotename(newnote->get_name()) < 0) 
		return 0;
	return ordered_insert(notelist, newnote, &note_data::get_position);
}

bool riffdata::delete_note(note_data * nd)
{	
	if (!nd) 
		return false;
	ll_item* tmp = notelist->find_data(nd);
	if (!tmp)
		return false;
	delete notelist->unlink_item(tmp);
	delete nd;
	return true;
}

#endif
