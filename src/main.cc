//-----------------------------------------------------------------------|
//                                                                       |
//                     O---\      [:::>  <:::]      /---O                |
//                     \ ----------   wcnt   ---------- /                |
//                          Wav Composer Not Toilet!                     |
//                     / ----------   1.25   ---------- \                |
//                     O---/      [::>    <::]      \---O                |
//                                     ||                                |
//                          ========================                     |
//                          | 2006 James W. Morris.|                     |
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
void show_counts();

int main(int argc, char **argv)
{
    jwmsynth* thesynth = new jwmsynth(argc, argv);
    title(thesynth->get_version());
    if (   !thesynth->is_valid()       || !thesynth->scan_cl_options()
        || !thesynth->generate_synth() || !thesynth->connect_synth()
        || !thesynth->validate_synth() || !thesynth->execute_synth())
    {
        cout << thesynth->get_error_msg();
        cout << "\n... exiting program ...\n";
        delete thesynth;
        show_counts();
        return -1;
    }
    delete thesynth;
    exitramblings();
    show_counts();
    return 0;
}

void show_counts()
{
    #ifdef SHOW_LL_ITEM_COUNT
    cout << "\nll_items created:  " << ll_item::get_created_count();
    cout << "\nll_items destroyed:" << ll_item::get_destroyed_count();
    cout << "\nmost ll_items at one time: " << ll_item::get_max_count();
    #endif
    #ifdef SHOW_DOBJ_COUNT
    cout << "\ndata objects created:  " << dobj::get_created_count();
    cout << "\ndata objects destroyed:" << dobj::get_destroyed_count();
    cout << "\nmost data objects at one time:" << dobj::get_max_count();
    #endif
    #ifdef SHOW_MOD_COUNT
    cout << "\nsynthmods created:  " << synthmod::get_created_count();
    cout << "\nsynthmods destroyed:" << synthmod::get_destroyed_count();
    cout << "\nmost synthmods at one time:" << synthmod::get_max_count();
    #endif
    #ifdef SHOW_NOTE_COUNT
    cout << "\nnotes created:  " << note_data::get_created_count();
    cout << "\nnotes destroyed:" << note_data::get_destroyed_count();
    cout << "\nmost notes at one time:" << note_data::get_max_count();
    #endif
    cout << "\n";
}

void title(const char* version)
{
    cout << "\no----{                      }----o";
    cout << "\n \\-------==\\   wcnt   /==-------/";
    cout << "\n  -> Wav Composer Not Toilet! <-";
    cout << "\n /-------==/   " << version << "   \\==-------\\";
    cout << "\no----{                      }----o";
}

void exitramblings()
{
    cout << "\nFlush complete.  Please wash your &&s, ";
    cout << "naturally.\n";
    cout.flush();
}
