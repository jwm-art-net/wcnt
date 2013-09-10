#ifndef RIFFNODE_H
#define RIFFNODE_H

#include "riffdata.h"

/* 
	class riff_node -- 'ghost' one riff many times without duplicating riffdata.
	I could turn on multiple referencing in the linked list to do the same thing, 
	but then I'd need some other way of telling where to put the riff in the 
	sequence, or make riffs of nothing to fill in the gaps.
	I choose this way. I shall also design a global riff_list which does not
	use the riff_node, but if you've got more than one sequencer, it can choose
	riffs from the list ie riffs are n't tied to any particular sequencer.
	therefore, sequencer cannot use linkedlist->get_current() of riff etc, so, 
	sequencer will have to have it's own members to remember where in the riff 
	it's at, which note it's at.  nb one riff can be used by multiple sequencers,
	at the same time, or different times.
*/	

class riff_node 
{
 public:
	riff_node(){};
	riff_node(riffdata*, short barpos);
	~riff_node();
	short get_start_bar() { return (this == NULL) ? 0 : start_bar; }
	riffdata* get_riff_source() { return (this == NULL) ? NULL : riff_source; }
 private:
	short start_bar;
	riffdata* riff_source;
};

#endif
