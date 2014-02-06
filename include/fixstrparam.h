#ifndef FIXSTRPARAM_H
#define FIXSTRPARAM_H

#include "paramnames.h"

/*
//  some parameters in definition use a series of fixed strings
//  which are converted to enums to set the parameter. an example
//  being play_dir in the sampler module which can be set to 'fwd'
//  or 'rev'.
//  this class stores the parameter type which does so and a string
//  containing all the possibilities, seperated by '/'.
//  converts a substring to a numerical value. following the example
//  'fwd' would return 0, and 'rev' would return 1.
//  the purpose being to further generalise reading of parameters,
//  by removing categories which have to be handled in a specialised
//  manner, and replacing them with a single category - CAT_FIX_STR.
//  an entry cannot be added if the param type has already been added
*/

class fixstrparam
{
 public:
    fixstrparam(const char* str_list, int par_type);
    ~fixstrparam();
    const char* get_string_list() const         { return string_list; }
    int get_param_type() const {return param_type; }
    int get_substring_count() const;
    int get_substring_index(const char* substr) const;
    const char* get_substring(int index) const;
    bool operator()(int & pt) const {
        return param_type == pt;
    }
 private:
    const char* string_list;
    int param_type;
};

#endif
