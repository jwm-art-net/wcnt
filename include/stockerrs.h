#ifndef STOCKERRS_H
#define STOCKERRS_H

// maximum frequency modulation size (think lfo here.)
#define MAX_FMOD_SIZE 32.0

// how much to divide sample rate by to get maximum frequency
#define MAX_FREQ_SR_DIV 4

// largest feedback level
#define MAX_FEEDBACK 2.0

// used to get stock err_msg strings
// and for 'auto' error detection in params

// ***
//  'auto' error detection can only handle numerical errors in
//  a parameter value.
//
//  for it to work properly, the markers ERR_RANGE_FIRST, and 
//  ERR_RANGE_LAST should not be removed, and the numerical
//  error types should be within the two markers.
// ***

// the remaining error types have to be detected by
// the synthmod or dobj that needs validating.

// 'auto' error detection means:
//    for synthmod derrived classes, synthmodclass::validate()
//      calls modparamlist::validate(synthmod* this, PAR_TYPE, ERR_TYPE)
//      and invalidates and returns the ERR_TYPE if modparamlist
//      returned false.

//      synthmodclass::validate does the error checking
//      itself for non numerical errors.

// type1 must be lowest or highest, or anywhere between
// type2 must be higher than lowest and lower than highest
// type3 one way range checks (usually must be above)
// type4 non automated checks.

class stockerrs
{
public:
    enum ERR_TYPE
    {
      ERR_FIRST = 0,      // cause:
        ERR_NO_ERROR,       // no error

      ERR_RANGE_TYPE1,   // *** see notes ***
        ERR_RANGE_0_1,      // outside 0 ~ 1
        ERR_RANGE_M1_1,     // outside -1 ~ 1
        ERR_RANGE_CLIP,     // outside 1 ~ 32767
        ERR_RANGE_AMP,      // outside -32767 ~ 32767
        ERR_RANGE_FEED,     // outside feedback range prob -2 ~ 2
        ERR_RANGE_DEGS,     // outside 0.0 ~ 360.0
        ERR_RANGE_SEMI,     // outside -12.0 ~ 12.0
        ERR_RANGE_OCT,      // outside -24.0 ~ 8.0 - overly generous.
        ERR_RANGE_BPM,      // outside -500 ~ 500 (now relative)
        ERR_RANGE_BEAT,     // outside 2 ~ 32
        
// should be inside range errmsgs:
      ERR_RANGE_TYPE2,
        ERR_RANGE_0_1_IN,   // outside 0 ~ 1 or is 0 or 1
        ERR_RANGE_M1_1_IN,  // outside -1 ~ 1 or is -1 or 1
        ERR_RANGE_FMOD,     // outside freq mod range prob 0 ~ 15
        ERR_RANGE_FREQ,     // outside 0.0 ~ 1/4 samplerate or is either.

      ERR_TYPE3,
        ERR_NEGATIVE,       // < 0
// must be errmsgs:
        ERR_NEG_ZERO,       // <= 0
        ERR_ABOVE1,         // < 1.0

      ERR_TYPE4,            // *** see notes ***
        ERR_LESS_THAN,      // a < b
        ERR_ATLEAST2,       // must be atleast two (object)

// could not errmsgs:
        ERR_NO_OPEN,        // could not open file
        ERR_NO_ADD,         // could not add (object)

// is not a errmsgs:
        ERR_ISNOT,          // is not a (object type)
        ERR_NOTENAME,       // is not a notename

        ERR_INVALID_OBJECT, // invalid object type
        ERR_ERROR,          // non-stock error
      ERR_LAST
    };
    stockerrs();
    ~stockerrs();
    char const* get_err(ERR_TYPE);
    char const* get_prefix_err(ERR_TYPE);
    static bool check_type(ERR_TYPE);
private:
    char** errmsg;
    char** prefixmsg;
};

#endif
