#ifndef DEBUG_H
#define DEBUG_H

#if DEBUG
#include <cstdio>
#endif


#define WCNT_DEBUG(fmt, ...) \
do { \
    fprintf(stderr, "[%s:%5d: %s] " fmt, \
            __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
} while(0)

#ifdef DEBUG
#define D_BUG(fmt, ...) WCNT_DEBUG(fmt, ##__VA_ARGS__)
#else
#define D_BUG(...) ((void)0)
#endif


#ifdef DEBUG_CONNECT
#define D_CONNECT(fmt, ...) WCNT_DEBUG(fmt, ##__VA_ARGS__)
#else
#define D_CONNECT(...) ((void)0)
#endif

#ifdef DEBUG_LADSPA
#define D_LADSPA(fmt, ...) WCNT_DEBUG(fmt, ##__VA_ARGS__)
#else
#define D_LADSPA(...) ((void)0)
#endif

#ifdef DEBUG_NOTEEDIT
#define D_NOTEEDIT(fmt, ...) WCNT_DEBUG(fmt, ##__VA_ARGS__)
#else
#define D_NOTEEDIT(...) ((void)0)
#endif

#ifdef DEBUG_UIITEM
#define D_UIITEM(fmt, ...) WCNT_DEBUG(fmt, ##__VA_ARGS__)
#else
#define D_UIITEM(...) ((void)0)
#endif

#endif
