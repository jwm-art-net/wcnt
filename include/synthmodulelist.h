/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef SYNTHMODULELIST_H
#define SYNTHMODULELIST_H

#ifndef BARE_MODULES

#include "synthmodule.h"
#include "synthmodnames.h"
#include "modinputslist.h"
#include "modoutputslist.h"

#include "modules.h"

class synthmodlist
{
 public:
	synthmodlist();
	~synthmodlist();
	synthmod* add_module(synthmod*);
	synthmod* create_module(synthmodnames::SYNTH_MOD_TYPE smt, string uname);
	bool delete_module(synthmod* const sm);
	synthmod* goto_first(){ return smod = (synthmod*)(smod_item = smodlist->goto_first())->get_data();}
	synthmod* goto_last(){ return smod = (synthmod*)(smod_item = smodlist->goto_last())->get_data();}
	synthmod* goto_prev(){ return smod = (synthmod*)(smod_item = smodlist->goto_prev())->get_data();}
	synthmod* goto_next(){ return smod = (synthmod*)(smod_item = smodlist->goto_next())->get_data();}
	synthmod* get_synthmod_by_name(string const*);
	synthmod* get_first_of_type(synthmodnames::SYNTH_MOD_TYPE);
	synthmod* get_next_of_type();
	
 private:
	linkedlist* smodlist;
	ll_item* smod_item;
	synthmod* smod;
	synthmodnames::SYNTH_MOD_TYPE search_modtype;
	ll_item* search_result;
};

#endif
#endif
