#include "../include/directivenames.h"

namespace directive
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,           "BAD_DIRECTIVE_TYPE",   iocat::ERR_TYPE,  "DIRECTIVE_ERROR"   ,0},
    { AUTOGROUP_CREATE,   "autogroup_create",     iocat::STRING,    "Name of group to create to autogroup modules into." ,0},
    { AUTOGROUP_ADD,      "autogroup_add",        iocat::STRING,    "Name of existing group to autogroup additional modules into." ,0},
    { AUTOGROUP_STOP,     "autogroup_stop",       iocat::STRING,    "Name of group to stop autogroup modules into." ,0}
 };
}; // namespace directive
