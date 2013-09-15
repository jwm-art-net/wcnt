#ifndef IOCATCONVERT_H
#define IOCATCONVERT_H

#include <fstream>

#include "iocat.h"

namespace iocatconv
{

void* cstr_to_iocat(iocat::IOCAT,   char const* cstrval,
                    std::ostringstream* result);

void destroy_iocat_data(iocat::IOCAT iocat, void* data);

const char* get_error_msg();

} // namespace iocat

#endif
