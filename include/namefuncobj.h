#ifndef NAMEFUNCOBJ_H
#define NAMEFUNCOBJ_H

#include <string>

/*
// function object for use with linkedlist template search functions
//  - for example: 
//      searching for synthmodule x named "summod" in synthmodlist
//
// see also: groupnames.h
*/

class name
{
 public:
    name(char* n) : _name(n) {};
    name(char const* n) : _name(n) {};

    bool operator ()(char* n) const {
        if (!n)
            return false;
        return strcmp(_name, n) == 0;
    }

    bool operator ()(char const* n) const {
        if (n == 0)
            return false;
        return strcmp(_name, n) == 0;
    }

 private:
    char const* _name;
};

class groupname
{
 public:
    groupname(char* n) : _name(n) {};
    groupname(char const* n) : _name(n) {};

    bool operator ()(char* n) const {
        if (!n)
            return false;
        return strcmp(_name, n) == 0;
    }

    bool operator ()(char const* n) const {
        if (!n)
            return false;
        return strcmp(_name, n) == 0;
    }

 private:
    char const* _name;
};

#endif
