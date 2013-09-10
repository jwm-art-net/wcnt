#ifndef GROUPNAMES_H
#define GROUPNAMES_H

#include <string.h>

// finds the group name portion of the name passed.
// returns it as new char[] if it exists, otherwise 0.
char* get_groupname(char const* name);

// adds or changes the group name.
// returns the new name as a new char[].
char* set_groupname(char const* groupname, char const* name);

#endif
