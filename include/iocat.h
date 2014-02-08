#ifndef IOCAT_H
#define IOCAT_H

#include "getnames.h"

// Used by triggers and switches and states

// every input, output, and parameter, will be of a category from this
// list... and then...

namespace iocat
{
 enum TYPE
 {
    ERR_TYPE,
    //----------------------------------------------------------------
    // the following types cover all inputs and outputs:
    //----------------------------------------------------------------
    DOUBLE,     // signal, bpm, etc
    WCINT_T,    // bar count, etc
    SAMP_T,     // sample positions etc
    TRIG,       // trigger      - ON/OFF - on for one sample only
    STATE,      // power/status - ON/OFF
    STRING,     // note_tran & sequencer employ this as input/output
    //----------------------------------------------------------------
    // these have been added as extra types for parameter
    // names and won't be used by inputs or outputs:
    // note: above statement is subject to change...
    //----------------------------------------------------------------
    FIX_STR,    // fixed string defined by whatevahhhh
    METER,      // time signature  ie 4/4 3/4 etc
    // the following types are read as strings but they need to match
    // the name of module/object
    DOBJ,       // looks up data from dobjlist
    SYNTHMOD,   // looks up data from smodlist
    // cat dobj and cat synthmod have arisen because of new code to
    // read data types ie riffs, adsrs, envelopes, tracks etc rather
    // than it being specialistically hard coded as before.
    SPECIAL,    // special type!
    //----------------------------------------------------------------
    // keep last:
    LAST_TYPE
 };

 class names : public getnames<TYPE, void*>
 {
  public:
    static void instantiate() { static names iocatnames; }

  private:
    names() : getnames(LAST_TYPE, data) {}
    ~names() {}

    static const struct gn_data data[LAST_TYPE];
 };
}; // namespace iocat

#endif
