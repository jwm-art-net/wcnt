#ifndef RIFFDATA_HH
#include "../include/riffdata.h"

riffdata::riffdata(string& uname): username(0), note(0), notelist(0)
{
	notelist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
	setnobars(1);
	username = new string(uname);
}

riffdata::~riffdata() 
{
	if (!notelist)
		return;
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
	if (!notelist)
		return 0;
	if (!newnote) 
		return 0;
	if (checknotename(newnote->get_name()) < 0) 
		return 0;
	return ordered_insert(notelist, newnote, &note_data::get_position);
}

bool riffdata::delete_note(note_data * nd)
{	
	if (!notelist) 
		return false;
	if (!nd) 
		return false;
	ll_item* tmp = notelist->find_data(nd);
	if (!tmp)
		return false;
	delete notelist->unlink_item(tmp);
	delete nd;
	return true;
}


void riffdata::setnobars(char b) 
{
	bars = calculate_noof_bars();
	if (b > bars)
		bars = b;
	riff_length = bars * beats_per_measure * (256 / beat_value);
}

char riffdata::getnobars()
{
	char b = calculate_noof_bars();
	if (b > bars)
		bars = b;
	riff_length = bars * beats_per_measure * (256 / beat_value);
	return bars;
}

short riffdata::getrifflength()
{
	char b = calculate_noof_bars();
	if (b > bars)
		bars = b;
	riff_length = bars * beats_per_measure * (256 / beat_value);
	return riff_length;
}

char riffdata::calculate_noof_bars() 
{	/* private helper method */
	if (!notelist) 
		return false;
	note_data* nd = get_last();
	double poslendiv = (nd->get_position() + nd->get_length()) / 256;
	return 1 + (char)poslendiv;
	// I did have the poslendiv calculation in the return statement
	// but the cast to char caused notes between pos 256 and 512 to be 'lost'
	// as noof of bars was always 1 :
	// return 1 + (char)(nd->get_position() + nd->get_length()) / 256
	// and in the test riff, the length of the note at position before bar 1 (256)
	// was after bar 1 (into bar 2) so was never turned off
}

#endif
