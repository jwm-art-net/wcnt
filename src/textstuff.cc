#include "../include/textstuff.h"

#include <functional>
#include <algorithm>

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>


std::string*
collumnize(const char** textarr,    unsigned int arrsize,
           unsigned int colwidth,   unsigned int colcount,  bool sort)
{
    unsigned int aix, i;
    unsigned int cw = 0;
    unsigned long len = 0;
    unsigned long maxlen = 0;
    for(aix = 0; aix < arrsize; aix++)
        if ((len = strlen(textarr[aix])) > maxlen)
            maxlen = len;
    for(i = 1; i < colcount; i++){
        if (maxlen < (cw = colwidth * i))
            break;
    }
    maxlen = cw;
    char** list = new char*[arrsize];
    for(aix = 0; aix < arrsize; aix++){
        list[aix] = new char[maxlen + 1];
        len = strlen(textarr[aix]);
        for (i = 1; i < colcount; i++){
            if (len < (cw = colwidth * i)) {
                break;
            }
        }
        strncpy(list[aix], textarr[aix], len);
        if (cw > len) {
            for (i = len; i < cw; i++)
                list[aix][i] = ' ';
            list[aix][cw] = '\0';
        }
    }
    if (sort == true) {
        std::sort(list, list + arrsize, std::less<std::string>());
        // and arrange in collumns:
        unsigned int colheight = (arrsize / colcount) + 1;
        unsigned int gridsize = colheight * colcount;
        char*** collumn = new char**[colcount];
        unsigned int row = 0, col = 0;
        for(col = 0; col < colcount; col++){
            collumn[col] = new char*[colheight];
        }
        col = 0;
        for(aix = 0; aix < gridsize; aix++){
            if (row == colheight){
                row = 0;
                col++;
            }
            if (aix < arrsize)
                collumn[col][row] = list[aix];
            else
                collumn[col][row] = 0;
            row++;
        }
        std::string* str = new std::string;
        *str = "";
        for(row = 0; row < colheight; row++){
            for(col = 0; col < colcount; col++){
                if (collumn[col][row]) {
                    *str += collumn[col][row];
                }
                else {
                    col = colcount;
                }
            }
            *str += "\n";
        }
        for(col = 0; col < colcount; col++)
            delete [] collumn[col];
        delete [] collumn;
        for(aix = 0; aix < arrsize; aix++)
            delete [] list[aix];
        delete [] list;
        return str;
    }
    std::string* str = new std::string;
    *str = "\n";
    unsigned int col = 1;
    for (aix = 0; aix < arrsize; aix++) {
        if (list[aix]) {
            len = strlen(list[aix]);
            cw = len / colwidth;
            if (col > colcount || (cw + col) > colcount + 1) {
                col = 1;
                *str += "\n";
            }
            col += cw;
            *str += list[aix];
            delete [] list[aix];
        }
    }
    delete [] list;
    return str;
}

size_t cfmt(char* buf, size_t len, const char* fmt, ...)
{
    char tmp[len + 1];
    va_list args;
    va_start(args, fmt);
    size_t n = vsnprintf(tmp, len, fmt, args);
    va_end(args);
    strncpy(buf, tmp, len);
    return n;
}

