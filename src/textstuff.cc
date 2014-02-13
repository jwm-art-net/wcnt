#include "../include/textstuff.h"

#include <functional>
#include <algorithm>

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>


std::string*
collumnize(const char** textarr, int arrsize, int colwidth, int colcount,
                                                                bool sort)
{
    int aix, i;
    int cw = 0;
    int len = 0;
    int maxlen = 0;
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
        int colheight = (arrsize / colcount) + 1;
        int gridsize = colheight * colcount;
        char*** collumn = new char**[colcount];
        int row = 0, col = 0;
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
    int col = 1;
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

std::string*
justify(const char* src, int width, int splitchar, const char* seperator,
                                                   const char* lead_in)
{
    if (width < 8 || !splitchar || !seperator)
        return 0;

    const char* p1 = src;
    std::string* str = new std::string;
    size_t w = width;

    if (lead_in) {
        w = width - strlen(lead_in);
        *str = lead_in;
    }

    do {
        char buf[w + 1];
        char* p2;
        strncpy(buf, p1, w);
        buf[w] = '\0';
        size_t l = strlen(buf);
        if (l == w) {
            p2 = buf + strlen(buf);
            while (*p2 != splitchar && p2 > p1)
                --p2;
            if (p2 == p1)
                p2 += w;
            else
                ++p2;
            *p2 = '\0';
            *str += buf;
            *str += seperator;
        }
        else
            *str += buf;
        p1 += strlen(buf);

        if (lead_in) {
            w = width;
            lead_in = 0;
        }
    } while(strlen(p1) > (size_t)width);
    *str += p1;
    return str;
}


size_t cfmt(char* buf, size_t bufsz, const char* fmt, ...)
{
    char tmp[bufsz];
    va_list args;
    va_start(args, fmt);
    size_t n = vsnprintf(tmp, bufsz - 1, fmt, args);
    va_end(args);
    strncpy(buf, tmp, bufsz - 1);
    buf[bufsz - 1] = '\0';
    return n;
}


char* sanitize_name(const char* src, const char* badchars, int replace, const char* prefix)
{
    debug("sanitizing string '%s' badchars '%s' replace '%c'\n", src, badchars, replace);

    size_t plen = (prefix != 0 ? strlen(prefix) : 0);
    size_t dlen = strlen(src) + 1 + plen + 1;

    char* str = new char[dlen * sizeof(char)];
    const char* s;
    char* d = str;
    bool skip = false;

    if (prefix) {
        for (s = prefix; *s != '\0'; ++s)
            *d++ = *s;
    }

    // replaces occurences of any character in 'badchars'
    // with character 'replace'.

    for (s = src; *s != '\0'; ++s) {
        bool repchar = false;
        if (*s == replace) {
            repchar = true;
            if (s == src)
                skip = true;
        }
        else {
            for (const char* bc = badchars; *bc != '\0'; ++bc) {
                if (*bc == *s) {
                    repchar = true;
                    break;
                }
            }
        }
        if (repchar) {
            *d = replace;
            if (!skip) {
                ++d;
                skip = true;
            }
        }
        else {
            *d = tolower(*s);
            skip = false;
            ++d;
        }
    }

    for (*d = '\0', --d; *d == replace; --d) {
        *d = '\0';
        --d;
    }

    if (prefix) {
        d = str;
        s = str + plen;
        if (strncmp(d, s, plen) == 0) {
            for (; *s != '\0'; ++s)
                *d++ = *s;
            *d = '\0';
        }
    }

    return str;
}


char* new_strdup(const char* src)
{
    if (!src)
        return 0;
    size_t l = strlen(src);
    char* dest = new char[(l + 1) * sizeof(char)];
    snprintf(dest, l + 1, "%s", src);
    return dest;
}


char* pretty_line(const char* segment, size_t len)
{
    size_t slen = strlen(segment);
    int count = (len / (double)slen) + 2;
    char* pl = new char[count * slen + 1];
    char* p = pl;
    while(count) {
        strcpy(p, segment);
        p += slen;
        --count;
    }
    return pl;
}


spaces::~spaces()
{
    if (data) {
        std::cout << "~spaces freeing data length: " << len << std::endl;
        delete [] data;
        data = 0;
        len = 0;
    }
}

const char* spaces::get(int n)
{
    static spaces blanks;
    if (n < 1)
        return 0;
    if (data) {
        if (n <= len) {
            return &data[len - n];
        }
        delete [] data;
        data = 0;
    }
    data = new char[n + 1];
    if (data) {
        for (int i = 0; i < n; ++i)
            data[i] = ' ';
        data[n] = '\0';
        len = n;
    }
    return data;
}

char* spaces::data = 0;
int spaces::len = 0;
