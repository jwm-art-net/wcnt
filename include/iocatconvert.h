#ifndef IOCATCONVERT_H
#define IOCATCONVERT_H

#include <sstream>

// included for CAT_DOBJ, CAT_SYNTHMOD, CAT_METER
#include "dobjlist.h"
#include "synthmodulelist.h"

namespace iocatconv
{

void* cstr_to_iocat(IOCAT, char const* cstrval, ostringstream* result);
void destroy_iocat_data(IOCAT iocat, void* data);

extern std::string err_msg;

} // namespace iocat

#endif
