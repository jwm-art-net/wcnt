#ifndef LISTWORK_H
#define LISTWORK_H

#include "linkedlist.h"

template <typename T, typename R>
ll_item<T>*
ordered_insert(linked_list<T>* list, T* data, R(T::*func)() const)
{
    ll_item<T>* tmp = list->sneak_first();
    if (!tmp)
        return list->add_at_head(data);
    ll_item<T>* after = 0;
    R needle = (data->*func)();
    while(tmp) {
        if ((tmp->get_data()->*func)() > needle) {
            if (!after)
                return list->add_at_head(data);
            else
                return list->insert_after(after, data);
        }
        after = tmp;
        tmp = tmp->get_next();
    }
    return list->add_at_tail(data);
}

template <typename T, typename R>
ll_item<T>*
ordered_insert_replace(linked_list<T>* list, T* data, R(T::*func)() const)
{
    ll_item<T>* tmp = list->sneak_first();
    if (!tmp)
        return list->add_at_head(data);
    ll_item<T>* after = 0;
    R needle = (data->*func)();
    while(tmp) {
        R value = (tmp->get_data()->*func)();
        if (value == needle) {
            tmp->set_data(data);
            return tmp;
        }
        else if (value > needle) {
            if (!after)
                return list->add_at_head(data);
            else
                return list->insert_after(after, data);
        }
        after = tmp;
        tmp = tmp->get_next();
    }
    return list->add_at_tail(data);
}


template <typename T, typename R>
ll_item<T>*
find_in_data(ll_item<T>* from, R needle)
{
    while (from) {
        if ((*from->get_data())(needle))
            return from;
        from = from->get_next();
    }
    return 0;
}

template <typename T, typename R>
ll_item<T>*
find_in_data_or_last_less_than(ll_item<T>* from, T* data,
                                                    R(T::*func)() const)
{
    R dvalue = (data->*func)();
    while (from) {
        R value = (from->get_data()->*func)();
        if (value > dvalue)
            return from->get_prev();
        else
        if (value == dvalue)
            return from;
        from = from->get_next();
    }
    return 0;
}

template <typename T, typename R>
size_t
copy_by_to_list(linked_list<T>* dest, linked_list<T>* src, R needle)
{
    size_t count = 0;
    ll_item<T>*
        tmp = find_in_data(src->sneak_first(), needle);
    while(tmp){
        if (dest->add_at_tail(tmp->get_data()))
            ++count;
        tmp = find_in_data(tmp->get_next(), needle);
    }
    return count;
}

template <typename T, typename R>
linked_list<T>*
new_list_of_by(linked_list<T>* list, R needle)
{
    ll_item<T>*
        tmp = find_in_data(list->sneak_first(), needle);
    linked_list<T>* newlist = new
        linked_list<T>(list->get_multiref(), PRESERVE_DATA);
    while(tmp){
        newlist->add_at_tail(tmp->get_data());
        tmp = find_in_data(tmp->get_next(), needle);
    }
    return newlist;
}

template <typename T, typename R>
linked_list<T>*
move_to_new_list_of_by(linked_list<T>* list, R needle)
{
    ll_item<T>*
        tmp = find_in_data(list->sneak_first(), needle);
    linked_list<T>* newlist = new
        linked_list<T>(list->get_multiref(), list->get_destruction());
    while(tmp){
        newlist->add_at_tail(tmp->get_data());
        ll_item<T>* del = tmp;
        tmp = find_in_data(tmp->get_next(), needle);
        delete list->unlink_item(del);
    }
    return newlist;
}

template <typename T>
T** move_to_array(linked_list<T>* list)
{
    if (list->is_empty())
        return 0;
    T** array = new T*[list->get_count() + 1];
    int ix = 0;
    ll_item<T>* tmp = list->sneak_first();
    while(tmp) {
        array[ix++] = tmp->get_data();
        tmp = tmp->get_next();
    }
    array[ix] = 0;
    list->empty_list(PRESERVE_DATA);
    return array;
}

template <typename T, typename R>
R** create_array_using(linked_list<T>* list, R*(T::*func)())
{
    if (list->is_empty())
        return 0;
    R** array = new R*[list->get_count() + 1];
    long ix = 0;
    ll_item<T>* tmp = list->sneak_first();
    while(tmp) {
        array[ix++] = (tmp->get_data()->*func)();
        tmp = tmp->get_next();
    }
    array[ix] = 0;
    return array;
}

template <typename T>
void destroy_array_moved_from_list(T** array)
{
    long ix = 0;
    while(array[ix]){
        delete array[ix++];
    }
    delete [] array;
}

template <typename T, typename R> // selection sort
void sort_list(linked_list<T>* list, R (T::*func)() const)
{
    ll_item<T>* expos = list->sneak_first();
    while(expos) {
        ll_item<T>* tmp = expos;
        ll_item<T>* smallest = tmp;
        while (tmp) {
            tmp = tmp->get_next();
            if (tmp && (tmp->get_data()->*func)()
              < (smallest->get_data()->*func)())
                smallest = tmp;
        }
        T* n = expos->get_data();
        expos->set_data(smallest->get_data());
        smallest->set_data(n);
        expos = expos->get_next();
    }
}

#endif
