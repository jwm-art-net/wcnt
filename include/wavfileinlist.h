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
 
#ifndef WAVFILEINLIST_H
#define WAVFILEINLIST_H

#ifndef BARE_MODULES

#include <string>
#include "linkedlist.h"
#include "wavfilein.h"

class wavfilein_list
{
 public:
	wavfilein_list();
	~wavfilein_list();
	wavfilein* goto_first(){ return wavfile = (wavfilein*)(wavfile_item = wavfilelist->goto_first())->get_data();}
	wavfilein* goto_last(){ return wavfile = (wavfilein*)(wavfile_item = wavfilelist->goto_last())->get_data();}
	wavfilein* goto_prev(){ return wavfile = (wavfilein*)(wavfile_item = wavfilelist->goto_prev())->get_data();}
	wavfilein* goto_next(){ return wavfile = (wavfilein*)(wavfile_item = wavfilelist->goto_next())->get_data();}
	wavfilein* goto_wavfile(wavfilein* rd){ return wavfile = (wavfilein*)(wavfile_item = wavfilelist->find_data(rd))->get_data();}
	wavfilein* add_wavfile(wavfilein* rd);
	bool delete_wavfile(wavfilein*);
	wavfilein* get_wavfilein_by_name(string const*);
	
 private:
	linkedlist* wavfilelist;
	ll_item* wavfile_item;
	wavfilein* wavfile;
};

#endif
#endif
