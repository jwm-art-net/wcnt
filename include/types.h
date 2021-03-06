#ifndef TYPES_H
#define TYPES_H

enum STATUS
{
    OFF = 0x0,
    ON  = 0x1
};

typedef long samp_t;
typedef unsigned long usamp_t;
typedef int  wcint_t;
typedef unsigned int  uwcint_t;

#define SAMP_T_MAX ((samp_t)(((usamp_t)-1) >> 2))
#define WCINT_T_MAX ((wcint_t)(((uwcint_t)-1) >> 2))


#define STRING_JIFFY(x) #x
#define TOSTRING(x) STRING_JIFFY(x)

#define STRBUFLEN 4000

#ifdef DATA_STATS
//------------------------------------------------------------------------
// Use the following macros for generating basic statistics about
// a class: number created, destroyed, most at once, and existing.

// this to be included in public section of class definition
#define STATS_FUNCS \
    static int get_current_count()    { return current_count; }\
    static int get_most_count()       { return most_count; }\
    static int get_total_created()    { return total_created; }\
    static int get_total_destroyed()  { return total_destroyed; }

// this to be included in private section of class definition
#define STATS_VARS \
    static int total_created;\
    static int current_count;\
    static int most_count;\
    static int total_destroyed;

// this to be included outside of any class definition or member
#define STATS_INIT(class) \
int class::total_created     = 0;\
int class::current_count     = 0;\
int class::most_count        = 0;\
int class::total_destroyed   = 0;

// this to be included in class constructor(s)
#define STATS_INC \
    current_count++;\
    total_created++;\
    if(current_count > most_count)\
        most_count = current_count;

// this to be included in class destructor
#define STATS_DEC \
    total_destroyed++;\
    current_count--;

#define STATS_DISPLAY(class) \
    std::cout << "\n------------------------------------\n";\
    std::cout << #class ;\
    std::cout << " statistics:";\
    std::cout << "\n\ttotal created    : "<<class::get_total_created();\
    std::cout << "\ttotal destroyed  : "<<class::get_total_destroyed();\
    std::cout << "\n\tcurrent count    : "<<class::get_current_count();\
    std::cout << "\tmost at one time : "<<class::get_most_count();\
    std::cout << "\n\tsizeof           : "<<sizeof(class);

#endif // DATA_STATS
#endif // TYPES_H
