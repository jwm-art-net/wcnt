#ifndef STOCKERRS_H
#define STOCKERRS_H

#include "getnames.h"

namespace errors
{
 enum CAT
 {
    CAT_ERR_TYPE,
    CAT_NO_ERROR,
    CAT_NUM_OUT_OF_RANGE,
    CAT_NUM_OUT_OF_RANGE_OR_EQ,
    CAT_NUM_BELOW,
    CAT_COULD_NOT,
    CAT_INVALID,
    CAT_OTHER,
    CAT_ERR_LAST
 };

 enum TYPE
 {
    ERR_TYPE,       // erroneous error!
    NO_ERROR,       // no error
    // CAT_NUM_OUT_OF_RANGE:
    RANGE_0_1,      // 0 ~ 1
    RANGE_M1_1,     // -1 ~ 1
    RANGE_FEED,     // prob -2 ~ 2
    RANGE_DEGS,     // 0.0 ~ 360.0
    RANGE_SEMI,     // -12.0 ~ 12.0
    RANGE_SEMI2,    // -24.0 ~ 24.0
    RANGE_OCT,      // -24.0 ~ 8.0 - overly generous.
    RANGE_BPM,      // -500 ~ 500 (now relative)
    RANGE_BEAT,     // 2 ~ 32
    RANGE_COUNT,    // 0 ~ WCINT_T_MAX
    RANGE_COUNT1,   // 1 ~ WCINT_T_MAX
    RANGE_SAMPLE,   // 0 ~ SAMP_T_MAX
    // CAT_NUM_OUT_OF_RANGE_OR_EQ:
    RANGE_0_1_IN,   // 0 ~ 1
    RANGE_M1_1_IN,  // -1 ~ 1
    RANGE_FMOD,     // 0 ~ 15
    RANGE_FREQ,     // 0.0 ~ 1/4
    // CAT_NUM_BELOW:
    NEGATIVE,       // < 0 is bad
    NEG_OR_ZERO,    // <= 0 is bad
    ABOVE1,         // < 1.0 is bad
    // CAT_OTHER:
    LESS_THAN,      // a < b
    ATLEAST2,       // must be atleast two (object)
    NO_OPEN,        // could not open file
    NO_ADD,         // could not add (object)
    ISNOT,          // is not a (object type)
    NOTENAME,       // is not a notename
    INVALID_OBJECT, // invalid object type
    ERROR,          // non-stock error (the error of a lazy man...)
    LAST_TYPE
 };

 class stock : public getnames<TYPE, CAT>
 {
  public:

    static const char* major;   // "*** MAJOR ERROR ***"
    static const char* bad;     // "Bad attempt made to"
    static const char* bad_add; // "Bad attempt made to add "
                                // "invalid object type"
    static const char* paramname; // unknown parameter name '%s'.

    static void instantiate() { static stock stockerrs; }

    static const char* get_prefix_msg(TYPE t)
        { return prefix_msg[data[chk(t)].cat]; }

  private:
    stock() : getnames(LAST_TYPE, data) {}
    ~stock() {}
    static const struct gn_data data[LAST_TYPE];
    static const char* prefix_msg[CAT_ERR_LAST];
 };
}; // namespace errors

#endif
