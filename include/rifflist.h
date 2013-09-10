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
 
#ifndef RIFFLIST_H
#define RIFFLIST_H

#include "riffdata.h"

class riff_list
{
 public:
	riff_list();
	~riff_list();
	riffdata* goto_first(){ return riff = (riffdata*)(riff_item = rifflist->goto_first())->get_data();}
	riffdata* goto_last(){ return riff = (riffdata*)(riff_item = rifflist->goto_last())->get_data();}
	riffdata* goto_prev(){ return riff = (riffdata*)(riff_item = rifflist->goto_prev())->get_data();}
	riffdata* goto_next(){ return riff = (riffdata*)(riff_item = rifflist->goto_next())->get_data();}
	riffdata* goto_riff(riffdata* rd){ return riff = (riffdata*)(riff_item = rifflist->find_data(rd))->get_data();}
	riffdata* add_riff(riffdata* rd);
	bool delete_riff(riffdata*);
	riffdata* get_riff_by_name(string const*);
	
 private:
	linkedlist* rifflist;
	ll_item* riff_item;
	riffdata* riff;
	riff_list(riff_list &rhs){};  // stop copying of riff_list
};

#endif
