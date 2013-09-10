#ifndef GROUPNAMES_H
#define GROUPNAMES_H

// finds the group name portion of the name passed.
// returns it as new char[] if it exists, otherwise 0.
const char* const get_groupname(const char* name);

// adds or changes the group name.
// returns the new name as a new char[].
char* set_groupname(char const* const groupname, char const* constname);

#endif
