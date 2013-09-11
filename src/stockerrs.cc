#include "../include/stockerrs.h"

stockerrs::stockerrs()
{
    errmsg[ERR_FIRST] = "*** PROGRAM ERROR - PROGRAMMER'S FAULT ***";
    errmsg[ERR_NO_ERROR] = "ok";

    errmsg[ERR_RANGE_TYPE1] = errmsg[ERR_FIRST];
    errmsg[ERR_RANGE_0_1] = "0.0 ~ 1.0";
    errmsg[ERR_RANGE_M1_1] = "-1.0 ~ 1.0";
    errmsg[ERR_RANGE_CLIP] = "1 ~ 32767";
    errmsg[ERR_RANGE_AMP] = "-32767 ~ 32767";
    errmsg[ERR_RANGE_FEED] = "-2.0 ~ 2.0";
    errmsg[ERR_RANGE_DEGS] = "0.0 ~ 360.0";
    errmsg[ERR_RANGE_SEMI] = "-12.0 ~ 12.0";
    errmsg[ERR_RANGE_OCT] = "-24.0 ~ 8.0";
    errmsg[ERR_RANGE_BPM] = "-500 ~ 500";
    errmsg[ERR_RANGE_BEAT] = "2 ~ 32";

    errmsg[ERR_RANGE_TYPE2] = errmsg[ERR_FIRST];
    errmsg[ERR_RANGE_0_1_IN] = "0.0 ~ 1.0";
    errmsg[ERR_RANGE_M1_1_IN] = "-1.0 ~ 1.0";
    errmsg[ERR_RANGE_FMOD] = "0.0 ~ 32.0";
    errmsg[ERR_RANGE_FREQ] ="0.0 ~ 1/4 sample rate";

    errmsg[ERR_TYPE3] = errmsg[ERR_FIRST];
    errmsg[ERR_NEGATIVE] = "is negative";// must not be
    errmsg[ERR_NEG_ZERO] = "above zero"; // must be
    errmsg[ERR_ABOVE1] = "above 1.0";    //  "   "

    errmsg[ERR_TYPE4] = errmsg[ERR_FIRST];
    errmsg[ERR_LESS_THAN] = "more than"; // must be
    errmsg[ERR_ATLEAST2] = "atleast two";//  "   "
    errmsg[ERR_NO_OPEN] = "opened"; // could not be
    errmsg[ERR_NO_ADD] = "added";   //   "    "  "
    errmsg[ERR_ISNOT] = " ";
    errmsg[ERR_NOTENAME] = "note name";
    errmsg[ERR_INVALID_OBJECT] = "invalid object type";
    errmsg[ERR_ERROR] = " ";

    prefixmsg[0] = " ";
    prefixmsg[1] = " out of range ";
    prefixmsg[2] = " should be inside range ";
    prefixmsg[3] = " must be ";
    prefixmsg[4] = " could not be ";
    prefixmsg[5] = " is not a ";
}

stockerrs::~stockerrs()
{
}

char const* stockerrs::get_err(stockerrs::ERR_TYPE et)
{
    if (et > ERR_FIRST && et < ERR_LAST)
        return errmsg[et];
    else
        return errmsg[ERR_FIRST];
}

char const* stockerrs::get_prefix_err(stockerrs::ERR_TYPE et)
{
    if (et > ERR_RANGE_TYPE1 && et < ERR_RANGE_TYPE2)
        return prefixmsg[1];
    if (et > ERR_RANGE_TYPE2 && et < ERR_TYPE3)
        return prefixmsg[2];
    if (et >= ERR_NEG_ZERO && et <= ERR_ATLEAST2)
        return prefixmsg[3];
    if (et >= ERR_NO_OPEN && et <= ERR_NO_ADD)
        return prefixmsg[4];
    if (et >= ERR_ISNOT && et <= ERR_NOTENAME)
        return prefixmsg[5];
    return prefixmsg[0];
}


bool stockerrs::check_type(ERR_TYPE et)
{
    switch (et)
    {
        case ERR_FIRST:
        case ERR_RANGE_TYPE1:
        case ERR_RANGE_TYPE2:
        case ERR_TYPE3:
        case ERR_TYPE4:
        case ERR_LAST:
            return false;
        default:
            return true;
    }
}

