//-----------------------------------------------------------------------|
//                                                                       |
//                     O---\\\\|||[:::>  <:::]|||////---O                |
//                     \ ----------   wcnt   ---------- /                |
//                          Wav Composer Not Toilet!                     |
//                     / ----------   v1.2   ---------- \                |
//                     O---////|||[::>    <::]|||\\\\---O                |
//                                     ||                                |
//                          ========================                     |
//                          | 2005 James W. Morris.|                     |
//                          ------------------------                     |
//                          |  Made In Stourmouth  |                     |
//                          ------------------------                     |
//                                                                       /
// contact->James_W.Morris( james@jwm-art.net or sirrom75@hotmail.com )  /
//                                                                       /
//----------------------------------------------------------------------*/

#include "../include/jwmsynth.h"

void title(const char* version);
void exitramblings();
void memerr(const char* msg);

int main(int argc, char **argv)
{
    jwmsynth* thesynth = new jwmsynth(argc, argv);
    title(thesynth->get_version());
    if (!thesynth->is_valid()) {
        cout << thesynth->get_error_msg();
        cout << "\n... exiting program ...\n";
        delete thesynth;
        return -1;
    }
    if (!thesynth->scan_cl_options()) {
        cout << thesynth->get_error_msg();
        cout << "\n... exiting program ...\n";
        delete thesynth;
        return -1;
    }
    if (!thesynth->generate_synth()) {
        cout << thesynth->get_error_msg();
        cout << "\n... exiting program ...\n";
        delete thesynth;
        return -1;
    }
    if (!thesynth->connect_synth()) {
        cout << thesynth->get_error_msg();
        cout << "\n... exiting program ...\n";
        delete thesynth;
        return -1;
    }
    if (!thesynth->execute_synth()) {
        cout << thesynth->get_error_msg();
        cout << "\n... exiting program ...\n";
        delete thesynth;
        return -1;
    }
    delete thesynth;
    exitramblings();
    return 0;
}

void title(const char* version)
{
    cout << "\n ___...---*  wcnt  *----...___";
    cout << "\n/__Wav Composer Not Toilet!__/";
    cout << "\n   /...---*  v";
    cout << version;
    cout << "  *---.../\n";
}

void exitramblings()
{
    cout << "\nFlush complete.  Please wash your &&s, ";
    cout << "naturally.\n";
    cout.flush();
}
