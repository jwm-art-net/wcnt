#ifndef IOCAT_H
#define IOCAT_H

#include "types.h"

// Used by triggers and switches and states

// every input, output, and parameter, will be of a category from this
// list

// I was going to remove the CAT_SHORT type, based on the removal of
// working with 16bit waveforms (ie passed through inputs/outpus) and
// parameters for amplitude levels etc.
// BUT, the CAT_SHORT is more appropriate for a number of other
// ins/out/params which I'd overlooked...so keep :)

class iocat
{
 public:
    enum IOCAT
    {
        // keep first:
        FIRST,
        //----------------------------------------------------------------
        // the following types cover all inputs and outputs:
        //----------------------------------------------------------------
        DOUBLE,     // signal, bpm, etc
        SHORT,      // bar count, etc
        ULONG,      // sample positions etc
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
        //----------------------------------------------------------------
        // keep last:
        LAST
    };
    iocat();
    ~iocat(){};
    const char* const get_name(IOCAT) const;
 private:
    struct iocat_data
    {
        IOCAT cat;
        const char* const name;
    };
    static const iocat_data data[LAST];
};

#endif
