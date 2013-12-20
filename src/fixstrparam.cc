#include "../include/fixstrparam.h"

#include <cstring>

static const char FXSTRSEP = '|';

fixstrparam::
fixstrparam(const char* str_list, param::TYPE par_type)
 : string_list(str_list), param_type(par_type)
{
}

fixstrparam::~fixstrparam()
{
    string_list = 0;
    param_type = param::ERR_TYPE;
}
//  "apple/pear/banana" apple = 0, pear = 1, banana = 2

int fixstrparam::get_substring_count() const
{
    if (!string_list)
        return 0;
    int substrcount = 1;
    const char* p = string_list;
    while(*p != '\0') {
        if (*p == FXSTRSEP && *(p + 1) != '\0')
            substrcount++;
        p++;
    }
    return substrcount;
}

int fixstrparam::get_substring_index(const char* substr) const
{
    if (substr == 0 || string_list == 0)
        return -1;
    int index = 0;
    char buf[80];
    char* bptr;
    const char* ptr = string_list;
    while(*ptr) {
        if (*ptr != FXSTRSEP) {
            bptr = buf;
            while (*ptr != FXSTRSEP && *ptr) {
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

const char* fixstrparam::get_substring(int index) const
{
    if (!string_list || index < 0)
        return 0;
    const char* p = string_list;
    const char* ps = 0;
    const char* pe = 0;
    if (index == 0)
        ps = p;
    int i = 0;
    while(*p != '\0'){
        if(*p == FXSTRSEP) {
            i++;
            if (i == index)
                ps = p + 1;
            else if (i > index && ! pe) {
                pe = p - 1;
                break;
            }
        }
        p++;
    }
    int len = 0;
    if (pe == 0)
        len = strlen(ps);
    else
        len = 1 + pe - ps;
    if (len < 1)
        return 0;
    char* substr = new char[len + 1];
    strncpy(substr, ps, len);
    substr[len] = '\0';
    return substr;
}

