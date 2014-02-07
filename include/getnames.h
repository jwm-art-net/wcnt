#ifndef GETNAMES_H
#define GETNAMES_H

#include <cstddef>
#include <cstring>


#include "linkedlist.h"
#include "listwork.h"
#include "function_objects.h"



template <typename T, typename C>
class getnames
{
 public:

    enum { EXTENDED = 0x1000 };

    #ifdef DEBUG
    static int chk(int t)
        { return (t > 0 && t < gn_count ? t : 0); }
    #else
    static T chk(int t) { return t; }
    #endif
    static int          type(const char* name);
    // static int          type(const char* name, T type_hint);
    static const char*  get(int t)      { return ((t & EXTENDED) ? ext_get(t)       : gn_data[chk(t)].name);    }
    static C category(int t)            { return ((t & EXTENDED) ? ext_category(t)  : gn_data[chk(t)].cat);     }
    static C category(const char* name) { int t = type(name);
                                          return ((t & EXTENDED) ? ext_category(t)  : gn_data[t].cat);          }
    static const char*  descr(int t)    { return ((t & EXTENDED) ? ext_descr(t)     : gn_data[chk(t)].descr);   }
    static const char** all_in_category(C cat, int* return_count);
    #ifdef UNUSED
    static int*         get_types_in_category(C cat);
    #endif

    static int          register_type(const char* name, C cat, const char* descr);

 protected:
    struct gn_data
    {
        int                 type;
        const char* const   name;
        C                   cat;
        const char* const   descr;
    };

    getnames(T count, const struct gn_data*);
    ~getnames();

 private:
    static const struct gn_data* gn_data;
    static int gn_count;
    #ifdef DEBUG
    static void check_index();
    #endif

    class extdata
    {
      public:
        extdata(int _type, const char* _name, C _cat, const char* _descr)
                : type(_type), name(_name), cat(_cat), descr(_descr)
                {}
        ~extdata(){};
        bool operator()(fnobj::name & n) const { return n(name); }
        int                 type;
        const char* const   name;
        C                   cat;
        const char* const   descr;
    };

    static linked_list<extdata>* ext;
    static int          ext_type(const char* name);
    static const char*  ext_get(int t);
    static C            ext_category(int t);
    static const char*  ext_descr(int t);
};

template <typename T, typename C>
getnames<T, C>::getnames(T count, const struct gn_data* data)
{
    gn_data = data;
    gn_count = count;
    #ifdef DEBUG
    check_index();
    #endif
    ext = 0;
}

template <typename T, typename C>
getnames<T, C>::~getnames()
{
    if (ext)
        delete ext;
}

template <typename T, typename C>
int getnames<T, C>::type(const char* name)
{
    for (int i = 1; i < gn_count; ++i)
        if (strcmp(name, gn_data[i].name) == 0)
            return i;
    return 0;
}

/*
template <typename T, typename C>
int getnames<T, C>::type(const char* name, T type_hint)
{
    if (strcmp(name, gn_data[chk(type_hint)].name) == 0)
        return type_hint;
    return type(name);
}
*/

#ifdef UNUSED
template <typename T, typename C>
int* getnames<T, C>::get_types_in_category(C cat)
{
    if (!cat)
        return 0;
    int count = 0;
    for (int i = 0; i < gn_count; ++i)
        if (gn_data[i].cat == cat)
            ++count;
    T* ret = new T[count + 1];
    if (!ret)
        return 0;
    count = 0;
    for (int i = 0; i < gn_count; ++i)
        if (gn_data[i].cat == cat)
            ret[count++] = i;
    ret[count] = 0;
    return ret;
}
#endif

template <typename T, typename C>
const char** getnames<T, C>::all_in_category(C cat, int* ret_count)
{
    if (ret_count)
        *ret_count = 0;
    if (!cat)
        return 0;
    int count = 0;
    for (int i = 0; i < gn_count; ++i)
        if (gn_data[i].cat == cat)
            ++count;
    const char** ret = new const char*[count + 1];
    if (!ret)
        return 0;
    count = 0;
    for (int i = 0; i < gn_count; ++i)
        if (gn_data[i].cat == cat)
            ret[count++] = gn_data[i].name;
    ret[count] = 0;
    if (ret_count)
        *ret_count = count;
    return ret;
}


#ifdef DEBUG
#include <iostream>
template <typename T, typename C>
void getnames<T, C>::check_index()
{
    for (int i = 0; i < gn_count; ++i) {
        if (gn_data[i].type != i) {
            std::cout << "***** getnames error *****" << std::endl;
            std::cout << "type mismatch for name " << gn_data[i].name
                      << " type " << gn_data[i].type
                      << "should match index " << i << "." << std::endl;
        }
    }
}
#endif


template <typename T, typename C>
int getnames<T, C>::register_type(const char* name, C cat, const char* descr)
{
    static int next_type_id = EXTENDED + 1;

    int ret = type(name);

    if (ret)
        return ret;

    if (!ext) {
        ext = new linked_list<extdata>;
    }

    struct gn_data* data = new extdata(next_type_id++, name, cat, descr);

    if (!data)
        return 0;

    if (!ext->add_at_tail(data)) {
        delete data;
        return 0;
    }

    return data->type;
}

template <typename T, typename C>
int getnames<T, C>::ext_type(const char* name)
{
    extdata* xd = find_in_data(ext, fnobj::name(name));

    return 0;
}


template <typename T, typename C>
const char* getnames<T, C>::ext_get(int )
{
    return 0;
}


template <typename T, typename C>
C getnames<T, C>::ext_category(int )
{
    return (C)0;
}


template <typename T, typename C>
const char* getnames<T, C>::ext_descr(int )
{
    return 0;
}



template <typename T, typename C>
struct getnames<T, C>::gn_data const* getnames<T, C>::gn_data = 0;

template <typename T, typename C>
int getnames<T, C>::gn_count = 0;

template <typename T, typename C>
linked_list< struct getnames<T, C>::extdata >* getnames<T, C>::ext = 0;


#endif
