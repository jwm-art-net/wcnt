#ifndef TEXTSTUFF_H
#define TEXTSTUFF_H

#if DEBUG
#include <cstdio>
#endif

#include <string>

/*
// if collumnize is called with sort set to true, then it will sort
// the array of c-strings alphanumerically within each collumn such that
// top of the 2nd collumn follows on from the bottom of the 1st.
//
// it should be noted that when sort is used, the width of the collumns
// should be greater than the longest c-string in the array - otherwise it
// causes a great deal of nonsense of logic which must be avoided at all
// costs.
//
// if collumnize is called with sort set to false, any c-string whose
// width is greater than the collumn width and whose length would take
// it into a collumn greater than collumn count, then the row will be
// abandonded and the c-string will begin on the next row.

// delete the returned string when done with.
*/

std::string*
collumnize(const char** textarr, int arrsize, int colwidth, int colcount,
                                                                bool sort);

template <typename T, typename R>
char** c_str_arr_from_names(int first, int last, const char* (T::*func)(R))
{
    size_t count = last - first;

    if (count <= 0)
        return 0;

    char** strarr = new char*[count + 1];
    for (int i = first; i < last; ++i)
        strarr[i] = new_strdup(func((R)i));
    strarr[count] = 0;
    return strarr;
}

size_t cfmt(char* buf, size_t buffer_size, const char* fmt, ...);

std::string*
justify(const char* src, int width, int splitchar, const char* seperator,
                                                   const char* lead_in);

// sanitize_name, replace characters in src which match with those
// in badchars with character specified by replace. instances of the
// replace character adjaecent to each other are trimmed down to a single
// instance.
char* sanitize_name(const char* src, const char* badchars, int replace);


// new_strdup - returns a duplicate c-string on which delete and not free()
// must be used to deallocate the memory.
char* new_strdup(const char*);


class spaces
{
 public:
    static const char* get(int);
 private:
    spaces() {};
    ~spaces();
    static char* data;
    static int len;
};


#if DEBUG
/*  debug previously just #define'd itself as errmsg but i want to
    allow a high-level message routine (ie msg_log in libpetrifui)
    to #undefine errmsg without damaging debug messages.
 */
#define debug(...)                              \
{                                               \
    fprintf(stderr, "%40s:%5d  %-35s: ",        \
            __FILE__, __LINE__, __FUNCTION__);  \
    fprintf(stderr, __VA_ARGS__);               \
}
#else
#define debug(...)
#endif


#endif
