#ifndef FIXSTRPARAM_H
#include "../include/fixstrparam.h"

fixstrparam::
fixstrparam(char const* str_list, paramnames::PAR_TYPE par_type)
 : string_list(str_list), param_type(par_type)
{
}

fixstrparam::~fixstrparam()
{
    string_list = 0;
    param_type = paramnames::PAR_FIRST;
}
//  "apple pear banana" apple = 0, pear = 1, banana = 2

int fixstrparam::get_substring_index(char const* substr)
{
    if (substr == 0 || string_list == 0) return -1;
    int index = 0;
    char buf[80];
    char* bptr;
    char const* ptr = string_list;
    while(*ptr) {
        if (*ptr != '/') {
            bptr = buf;
            while (*ptr != '/' && *ptr) {
                *bptr = *ptr;
                bptr++;
                ptr++;
            }
            *bptr = '\0';
            if (strcmp(substr, buf) == 0)
                return index;
            if (*ptr == 0)
                return -1;
            index++;
        }
        ptr++;
    }
    return -1;
}

#endif
