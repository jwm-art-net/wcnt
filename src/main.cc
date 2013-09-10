//-------------------------------------------------------------------------------------
//	Created with Anjuta version 1.0.0 
//-------------------------------------------------------------------------------------
// 	copysigh jeeems margheeees toot_owl_sand_anty 
// 	 >--SEE && SEA +  James Morris + 2003--<
//	Made In England
//-------------------------------------------------------------------------------------
// contact->JamesMorris( sirrom75|at|hotmail|dot|com );
//-------------------------------------------------------------------------------------
// see my <lovely> paintings @ 
// www.britishpaintingforsale.com/James%20Morris.asp
//-------------------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "../include/jwmsynth.h"

void title();
void exitramblings();
void memerr(const char* msg);

int main(int argc, char **argv)
{
	title();
	jwmsynth thesynth(argc, argv);
	if (!thesynth.is_valid()) {
		cout << thesynth.get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth.scan_cl_options()) {
		cout << thesynth.get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth.generate_synth()) {
		cout << thesynth.get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth.connect_synth()) {
		cout << thesynth.get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth.execute_synth()) {
		cout << thesynth.get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	exitramblings();
	return 0;
}

void title()
{
	cout << "\n|||||||||||||||!||||||||||||||";
	cout << "\n------------------------------";
	cout << "\nwcnt - Wav Composer Not Toilet";
	cout << "\n------------------------------";
	cout << "\n||||||||||||||||||||||||||||||";
}

void exitramblings()
{
	cout << "\nNo sweat.... maybe it's mybassline\n";
}
