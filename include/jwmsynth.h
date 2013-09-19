#ifndef JWMSYNTH
#define JWMSYNTH

#include <string>

#include "types.h"

class synthfilereader;

class jwmsynth
{
 public:
    jwmsynth();
    ~jwmsynth();

    // must call these functions  in order stated
    bool is_valid() const { return valid;}// 1
    bool generate_synth();          // 2
    bool connect_synth();           // 3 note: original validate and init
    bool validate_synth();          // 4 code migrated to synthmodlist due
    bool init_synth();              // 5 to planned synthmodlist stealing
    bool execute_synth();           // 6 module...

    std::string get_error_msg() const { return err_msg;}

 private:
//--- dobj and synthmod static member variables ---//
    std::string* synthmod_err_msg;
    std::string* dobj_err_msg;
    synthfilereader* synthfile_reader;

//--- validation and error messages ---//
    bool valid;
    std::string err_msg;

//--- when to stop ---//
    short exit_bar;
    const STATUS* in_bar_trig;
    const short*  in_bar;
};

#endif
