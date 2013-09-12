#ifndef GROUPNAMES_H
#define GROUPNAMES_H

#include "namefuncobj.h"

/*
// get_groupname
//  finds the group name portion of the name passed.
//  returns it as new char[] if it exists, otherwise 0.
*/
const char* get_groupname(const char* name);

/*
// set_groupname
//  adds or changes the group name.
//  returns the new name as a new char[].
*/
char* set_groupname(char const* const groupname, char const* constname);


#endif
