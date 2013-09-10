#ifndef SETPARAM_H
#define SETPARAM_H

#include "iocatconvert.h"

/*  these were formerly part of synthfilereader read_mod_params(),
    and read_dobj_params, but have been removed to avoid duplication
    elsewhere.

    const char* param is what was read as part of definition
    paramnames::PAR_TYPE pt is the param type expected by definition
    const char* value is the string value read
    ostringstream* result on return contains the result of the conversion
    to the data type of the parameter, converted back again.

    the functions return true if successful, and false if not.
    if unsuccessful, err_msg describes what went wrong.
*/

namespace setpar
{

bool set_mod_param(synthmod* module, const char* param,
 paramnames::PAR_TYPE pt, const char* value, ostringstream* result);

bool set_dobj_param(dobj* dbj, const char* param,
 paramnames::PAR_TYPE pt, const char* value, ostringstream* result);

extern std::string err_msg;

} // namespace setpar;

#endif
