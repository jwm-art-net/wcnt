#ifndef IOCATCONVERT_H
#define IOCATCONVERT_H

#include <fstream>

#include "iocat.h"

namespace iocatconv
{

void* cstr_to_iocat(iocat::TYPE,   const char* cstrval,
                    std::ostringstream* result);

void destroy_iocat_data(iocat::TYPE iocat, const void* data);

const char* get_error_msg();

} // namespace iocat

#endif
