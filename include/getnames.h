#ifndef GETNAMES_H
#define GETNAMES_H


#include <cstring>


template <typename T, typename C>
class getnames
{
 public:
    static T            type(const char* name);
    static const char*  get(T type)      { return gn_data[type].name;  }
    static C            category(T type) { return gn_data[type].cat;   }
    static const char*  descr(T type)    { return gn_data[type].descr; }

 protected:
    struct gn_data
    {
        T                   type;
        const char* const   name;
        C                   cat;
        const char* const   descr;
    };

    getnames(T count, const struct gn_data*);
    ~getnames(){};

 private:
    static const struct gn_data * gn_data;
    static T gn_count;
    #ifdef DEBUG
    static void check_types();
    #endif
};

template <typename T, typename C>
getnames<T, C>::getnames(T count, const struct gn_data* data)
{
    gn_data = data;
    gn_count = count;
    #ifdef DEBUG
    check_types();
    #endif
    
}

template <typename T, typename C>
T getnames<T, C>::type(const char* name)
{
    for (int i = 1; i < gn_count; ++i)
        if (strcmp(name, gn_data[i].name) == 0)
            return (T)i;
    return (T)0;
}

#ifdef DEBUG
#include <iostream>
template <typename T, typename C>
void getnames<T, C>::check_types()
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
struct getnames<T, C>::gn_data const* getnames<T, C>::gn_data = 0;

template <typename T, typename C>
T getnames<T, C>::gn_count;

#endif
