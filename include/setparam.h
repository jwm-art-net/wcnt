#ifndef SETPARAM_H
#define SETPARAM_H

#include <sstream>

#include "paramnames.h"

/*
//  these were formerly part of synthfilereader read_mod_params(),
//  and read_dobj_params, but have been removed to avoid duplication
//  elsewhere.
//  const char* param is what was read as part of definition
//  param::TYPE pt is the param type expected by definition
//  const char* value is the string value read
//  ostringstream* result on return contains the result of the conversion
//  to the data type of the parameter, converted back again.
//  the functions return true if successful, and false if not.
//  if unsuccessful, err_msg describes what went wrong.
*/

/*
// wcnt-1.26.4
// templatized some of this...
// and allowed relative param_edits via param_editor....
// note: how rock-steady this is not is quite a lot...
// i suspect..
//
// this is something of a bodge.
*/

/*
// N.B. I believe this exists mostly to serve the param_editor.
//      Oh no, my mistake, turns out synthfilereader is quite
//      smitten too.
*/


namespace setpar
{
 template <typename T>
 bool set_param(T* obj, const char* param, param::TYPE pt,
                        const char* value, std::ostringstream* result);

 template <typename T>
 void* compute(T* obj, param::TYPE pt, void* data, int op);

 //--------------------------------------------------------------
 // is_operator
 //      checks txt is a single character & terminated:
 //      detects  none         +   -   *   /
 //      returns false  -or-  true
 bool is_operator(const char* txt);

 //--------------------------------------------------------------
 // get_operator
 //      checks only first character of any-length string:
 //      detects none  +   -   *   /
 //      returns  0   '+' '-' '*' '/' (ie ascii char code).
 int get_operator(const char* txt);

 template <typename T>
 T comp(T* a, T*b, int op)
 {
    switch(op)
    {
        case '+': return *a + *b;
        case '-': return *a - *b;
        case '*': return *a * *b;
        case '/': return *a / *b;
        default: return 0;
    }
 }

 const char* get_error_msg();

}; // namespace setpar

#endif
