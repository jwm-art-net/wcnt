#ifndef GROUPNAMES_H
#include "../include/groupnames.h"

#include <stdio.h>
#include <string.h>

const char* const get_groupname(char const* const name)
{
    if (!name) return 0;
    char* chkname = new char[strlen(name) + 1];
    strcpy(chkname, name);
    char* ptr = chkname;
    while (*ptr != '\0') {
        if (*ptr == '.') {
            *ptr = '\0';
            char* groupname = new char[strlen(chkname) + 1];
            strcpy(groupname, chkname);
            *ptr = '.';
            delete [] chkname;
            return groupname;
        }
        ptr++;
    }
    delete [] chkname;
    return 0;
}

char* set_groupname(const char* const groupname, const char* const name)
{
    char* non_grp_name;
    char* tmp_name = new char[strlen(name ) + 1];
    strcpy(tmp_name, name);
    char* ptr = non_grp_name = tmp_name;
    while (*ptr != '\0') {
        if (*ptr == '.') {
            ptr++;
            non_grp_name = new char[strlen(ptr) + 1];
            strcpy(non_grp_name, ptr);
            break;
        }
        ptr++;
    }
    if (non_grp_name != tmp_name)
        delete [] tmp_name;
    int glen = strlen(groupname);
    int nlen = strlen(non_grp_name);
    int newlen = nlen + 1 + glen + 1;
    char* newname = new char[newlen];
    // stop valgrind from reporting conditional jumps on unitiated values:
    for (int i = 0; i < newlen; i++) newname[i] = 0;
    strncpy(newname, groupname, glen);
    strncpy(newname + glen + 1, non_grp_name, nlen);
    newname[glen] = '.';
    newname[newlen - 1] = '\0';
    delete [] non_grp_name;
    return newname;
}

#endif
