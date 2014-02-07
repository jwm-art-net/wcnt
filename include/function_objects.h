#ifndef FUNCTION_OBJECTS_H
#define FUNCTION_OBJECTS_H

#include <cstring>


namespace fnobj
{
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
    name(const char* n) : _name(n) {};

    bool operator ()(char* n) const {
        if (!n)
            return false;
        return strcmp(_name, n) == 0;
    }

    bool operator ()(const char* n) const {
        if (n == 0)
            return false;
        return strcmp(_name, n) == 0;
    }

  private:
    const char* _name;
 };


 class groupname
 {
  public:
    groupname(char* n) : _name(n) {};
    groupname(const char* n) : _name(n) {};

    bool operator ()(char* n) const {
        if (!n)
            return false;
        return strcmp(_name, n) == 0;
    }

    bool operator ()(const char* n) const {
        if (!n)
            return false;
        return strcmp(_name, n) == 0;
    }

  private:
    const char* _name;
 };

}; // namespace fnobj


#endif
