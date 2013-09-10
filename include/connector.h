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
 
#ifndef CONNECTOR_H
#define CONNECTOR_H

#ifndef BARE_MODULES

#include <string.h>
#include "synthmodulelist.h"

class connector
{
 public:
	connector(	synthmod* input_module, 
				inputnames::IN_TYPE input_type, 
				string output_module_name, 
				outputnames::OUT_TYPE output_type );
	~connector();
	synthmod* get_input_module(){ return in_mod;}
	inputnames::IN_TYPE get_input_type(){ return in_type;}
	string get_output_module_name(){ return out_mod_uname;}
	outputnames::OUT_TYPE get_output_type(){ return out_type;}
	// only do this once all synth modules have been created.
	// otherwise modules may reference as yet uncreated modules.
	// connect() would start crying if that happend.
	bool connect();
	static string const* get_connect_err_msg(){ return &connect_err_msg;}
 private:
	synthmod* in_mod;
	inputnames::IN_TYPE in_type;
	string out_mod_uname;
	outputnames::OUT_TYPE out_type;
	static string connect_err_msg;
};
#endif
#endif
