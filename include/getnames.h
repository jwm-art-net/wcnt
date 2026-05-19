#ifndef GETNAMES_H
#define GETNAMES_H


#include <cstring>


template <typename T, typename C, typename M = int>
class getnames
{
 public:

    #ifdef DEBUG
    static T chk(T t)
        { return ((int)t > 0 && (int)t < gn_count ? t : (T)0); }
    #else
    static T chk(T t) { return t; }
    #endif
    static T            type(const char* name);
    static T            type(const char* name, T type_hint);
    static const char*  get(T t)         { return gn_data[chk(t)].name;    }
    static C category(T t)               { return gn_data[chk(t)].cat;     }
    static C category(const char* name)  { return gn_data[type(name)].cat; }
    static const char*  descr(T t)       { return gn_data[chk(t)].descr;   }
    static M misc(T t)                   { return gn_data[chk(t)].misc;    }
    static const char** all_in_category(C cat, int* return_count);
    #ifdef UNUSED
    static T*           get_types_in_category(C cat);
    #endif

 protected:
    struct gn_data
    {
        T                   type;
        const char* const   name;
        C                   cat;
        const char* const   descr;
        M                   misc;
    };

    getnames(T count, const struct gn_data*);
    ~getnames(){};

 private:
    static const struct gn_data* gn_data;
    static int gn_count;
    #ifdef DEBUG
    static void check_index();
    #endif
};

template <typename T, typename C, typename M>
getnames<T, C, M>::getnames(T count, const struct gn_data* data)
{
    gn_data = data;
    gn_count = count;
    #ifdef DEBUG
    check_index();
    #endif
}

template <typename T, typename C, typename M>
T getnames<T, C, M>::type(const char* name)
{
    for (int i = 1; i < gn_count; ++i)
        if (strcmp(name, gn_data[i].name) == 0)
            return (T)i;
    return (T)0;
}

template <typename T, typename C, typename M>
T getnames<T, C, M>::type(const char* name, T type_hint)
{
    if (strcmp(name, gn_data[chk(type_hint)].name) == 0)
        return type_hint;
    return type(name);
}

#ifdef UNUSED
template <typename T, typename C, typename M>
T* getnames<T, C, M>::get_types_in_category(C cat)
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

template <typename T, typename C, typename M>
const char** getnames<T, C, M>::all_in_category(C cat, int* ret_count)
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
template <typename T, typename C, typename M>
void getnames<T, C, M>::check_index()
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

template <typename T, typename C, typename M>
struct getnames<T, C, M>::gn_data const* getnames<T, C, M>::gn_data = 0;

template <typename T, typename C, typename M>
int getnames<T, C, M>::gn_count = 0;

#endif
