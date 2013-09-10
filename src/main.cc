/*--------------------------------------------------------------------------|
//  initially Created with Anjuta version 1.0.0, now edited in version 1.2.0|
//--------------------------------------------------------------------------|
//                                                                          |
//                     O---\\\\|||[:::>  <:::]|||////---O                   |
//                     \ ----------   wcnt   ---------- /                   |
//                          Wav Composer Not Toilet!                        |
//                     / ----------  v 1.1z  ---------- \                   |
//                     O---////|||[::>    <::]|||\\\\---O                   |
//                                     ||                                   |
//                          ========================                        |
//                          | 2004 James W. Morris.|                        |
//                          ------------------------                        |
//	                        |    Made In England   |                        |
//                          ------------------------                        |
//                                                                          /
// contact->James_W.Morris( james@jwm-art.net )                            /
//                                                                        /
//----------------------------------------------------------------------*/

#ifdef BARE_MODULES
#include "../include/synthmodule.h"
#include "../include/modules.h"
#else
#include "../include/jwmsynth.h"
#endif

void title(const char* version);
void exitramblings();
void memerr(const char* msg);

#ifdef BARE_MODULES	
void moduletest();
#endif

int main(int argc, char **argv)
{
	#ifdef BARE_MODULES
	moduletest();
	#else
	jwmsynth* thesynth = new jwmsynth(argc, argv);
	title(thesynth->get_version());
	if (!thesynth->is_valid()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->scan_cl_options()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->generate_synth()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->connect_synth()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	if (!thesynth->execute_synth()) {
		cout << thesynth->get_error_msg();
		cout << "\n... exiting program ...\n";
		return -1;
	} 
	#endif // BARE_MODULES
	exitramblings();
	return 0;
}

void title(const char* version)
{
	cout << "\n0---^^^^|||[###>  <###]|||^^^^---O";
	cout << "\n\\ ----------   wcnt   ---------- /";
	cout << "\n     Wav Composer Not Toilet!";
	cout << "\n/ ----------  v ";
	cout << version;
	cout << "  ---------- \\";
	cout << "\nO---^^^^|||[##>    <##]|||^^^^---O\n";
}

void exitramblings()
{
	cout << "\nFlush complete.  Please wash your &&s.\n";
}

#ifdef BARE_MODULES
void moduletest()
{
	// place module testing code here...
	cout << "\nDone testing...\n";
}
#endif
