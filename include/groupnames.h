#ifndef GROUPNAMES_H
#define GROUPNAMES_H

#include "namefuncobj.h"
#include "debug.h"
#include <cstdio>
/*
// get_groupname
//  finds the group name portion of the name passed.
//  returns it as new char[] if it exists, otherwise 0.
*/
//char* get_groupname(const char* name);

/*
// set_groupname
//  adds or changes the group name.
//  returns the new name as a new char[].
*/
//char* set_groupname(const char* const groupname, const char* constname);

//bool has_groupname(const char* const name);




// proposed module naming convention
//  voice:group.chain~module
//
// set_name(VOICE|GROUP|CHAIN|MODULE, "name");
// get_name(VOICE|GROUP|CHAIN|MODULE|USER);
//
// voice component indicated by presence of ':'
// group component indicated by presence of '.'
// chain component indicated by presence of '~'

// user component is the full username


class groupnames
{
 public:
    enum COMPONENT { USER, VOICE, GROUP, CHAIN, MODULE };
    groupnames();
    groupnames(COMPONENT, const char*);
    ~groupnames();

    const char* set_name(COMPONENT, const char*);
    const char* set_username(const char*);
    bool        has_name(COMPONENT c) const { return components[c] != 0; }
    const char* get_name(COMPONENT c) const { return components[c]; }
    const char* get_username() const        { return components[0];}

    // make_username does not set the specified name component (note const)
    char*       make_username(COMPONENT, const char*) const;

    // static member functions for use without groupnames instance
    static char* str_make_username(const char* str, COMPONENT c,
                                   const char* compstr);
    static char* str_make_username(const char* voice, const char* group,
                                   const char* chain, const char* module);
    static char* str_get_name(COMPONENT c, const char*);
    static bool  str_has_name(COMPONENT c, const char*);
    static int   str_has_reserved_chr(const char*);

private:
    char* components[MODULE + 1];
    static char seperators[MODULE + 1];
};


/*
 i used this code to debug a nefarious issue caused by a inopportune delete []
 that i found very difficult to track down!
 set a breakpoint on the line char pppp = 1, and the if statement enclosing
 it detects when first character is not a lowercase alphanumeric value.

D_CONNECT("c0: '%s'  c1: '%s'  c2: '%s' (%p)  c3: '%s'  c4: '%s'\n",
          components[0], components[1], components[2], components[2], components[3], components[4]);
char ch = components[2] ? *components[2] : 'a';
if ((ch < 'a' || ch > 'z') && (ch < '0' || ch > '9')) {
    char pppp = 1;
}
*/

#endif
