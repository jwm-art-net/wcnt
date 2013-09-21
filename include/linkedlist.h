#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "types.h"

/*
// reworked...
//  it might be aswell to say this linkedlist is implimented
//  without consideration given to anything other than pointers
//  to classes.
//
// adding a pointer to 0 will fail
//
// this might become a pure abstract base class too. who knows?
// (i don't like that idea, i want this to be as fast as possible).
//
// but this class DOES NEED to be inherited if i wish to keep everything
// except the navigation functions hidden from the outside world...
//
*/


enum MULTIREF
{
    MULTIREF_OFF,
    MULTIREF_ON
};

enum DESTRUCTION
{
    PRESERVE_DATA,
    DELETE_DATA
};

template <typename T>
class ll_item
{
 public:
    ll_item(T* d) : data(d), prev(0), next(0) {}

    ~ll_item() {}

    void set_prev(ll_item* p) { prev = p; }
    void set_next(ll_item* n) { next = n; }
    void set_data(T* d)       { data = d; }
    T* get_data()       { return (this ? data : 0); }
    ll_item* get_prev() { return (this ? prev : 0); }
    ll_item* get_next() { return (this ? next : 0); }

 private:
    T*       data;
    ll_item* prev;
    ll_item* next;
};

template <typename T>
class linked_list
{
 public:

    linked_list(MULTIREF m = MULTIREF_OFF, DESTRUCTION d = DELETE_DATA);
    ~linked_list();

    typedef ll_item<T>      llitem;
    typedef linked_list<T>  linkedlist;
    typedef T datatype;

    T* goto_first(){
        return (current = head->get_next())->get_data();
    }
    T* goto_last() {
        return (current = tail->get_prev())->get_data();
    }
    T* goto_prev() {
        return (current = current->get_prev())->get_data();
    }
    T* goto_next() {
        return (current = current->get_next())->get_data();
    }

    T* goto_first() const {
        return (current = head->get_next())->get_data();
    }
    T* goto_last() const {
        return (current = tail->get_prev())->get_data();
    }
    T* goto_prev() const {
        return (current = current->get_prev())->get_data();
    }
    T* goto_next() const {
        return (current = current->get_next())->get_data();
    }

    bool is_empty(){ return (head->get_next() ? false : true); }

    MULTIREF get_multiref() const       { return multiref;}
    DESTRUCTION get_destruction() const { return destruction; }

    llitem* add_at_head(T* data);
    llitem* add_at_tail(T* data);

    llitem* sneak_first(){ return head->get_next(); }
    llitem* sneak_last() { return tail->get_prev(); }
    llitem* sneak_prev() { return current->get_prev(); }
    llitem* sneak_next() { return current->get_next(); }
    llitem* sneak_current() { return current; }

    void empty_list(DESTRUCTION d)
    {
        DESTRUCTION tmp = destruction;
        destruction = d;
        empty_list();
        destruction = tmp;
    }

    void empty_list() {
        llitem* tmp = head->get_next();
        while(tmp) {
            llitem* ntmp = tmp->get_next();
            if (destruction == DELETE_DATA)
                delete tmp->get_data();
            delete tmp;
            tmp = ntmp;
        }
        head->set_next(0);
        tail->set_prev(0);
    }

    llitem* unlink_item(llitem*);

    llitem* insert_after(llitem* after, T* data);

    llitem* find_item(llitem* item) {
        for(llitem* tmp = sneak_first();
            tmp != 0;
            tmp = tmp->get_next())
            if (tmp == item)
                return item;
        return 0;
    }

    llitem* find_data(llitem*from , T* data);

    long get_count() const  { return count; }

 private:
    llitem* head;
    llitem* tail;
    llitem* current;

    MULTIREF    multiref;
    DESTRUCTION destruction;

    long count;
};


template <typename T>
linked_list<T>::linked_list(MULTIREF m, DESTRUCTION d) :
 head(0), tail(0), current(0),
 multiref(m),
 destruction(d),
 count(0)
{
    head = new llitem(0);
    tail = new llitem(0);
}

template <typename T>
linked_list<T>::~linked_list()
{
    empty_list();
    delete head;
    delete tail;
}

template <typename T>
ll_item<T>* linked_list<T>::add_at_head(T* data)
{
    if (!data)
        return 0;
    if (multiref == MULTIREF_OFF)
        if (find_data(sneak_first(), data))
            return 0;
    llitem* newitem;
    if (!head->get_next()) {
        newitem = new llitem(data);
        head->set_next(newitem);
        tail->set_prev(newitem);
    }
    else {
        llitem* tmp;
        newitem = new llitem(data);
        tmp = head->get_next();
        newitem->set_next(tmp);
        tmp->set_prev(newitem);
        head->set_next(newitem);
    }
    count++;
    return current = newitem;
}

template <typename T>
ll_item<T>* linked_list<T>::add_at_tail(T* data)
{
    if (!data)
        return 0;
    if (multiref == MULTIREF_OFF) {
        if (find_data(sneak_first(), data))
            return 0;
    }
    llitem* newitem;
    if (!head->get_next()) {
        newitem = new llitem(data);
        tail->set_prev(newitem);
        head->set_next(newitem);
    }
    else {
        llitem* tmp;
        newitem = new llitem(data);
        tmp = tail->get_prev();
        newitem->set_prev(tmp);
        tmp->set_next(newitem);
        tail->set_prev(newitem);
    }
    count++;
    return current = newitem;
}

template <typename T>
ll_item<T>* linked_list<T>::insert_after(llitem* after, T* data)
{
    if (!after || !data || !head->get_next())
        return 0;  
    if (multiref == MULTIREF_OFF) {
        if (find_data(sneak_first(), data))
            return 0;
    }
    llitem* newitem;
    if (after == tail->get_prev()) {
        newitem = new llitem(data);
        newitem->set_prev(after);
        after->set_next(newitem);
        tail->set_prev(newitem);
    }
    else {
        newitem = new llitem(data);
        llitem* aftnxt = after->get_next();
        after->set_next(newitem);
        newitem->set_prev(after);
        newitem->set_next(aftnxt);
        aftnxt->set_prev(newitem);
    }
    #ifdef LIST_DEBUG
    newitem->set_owner(this);
    #endif
    count++;
    return current = newitem;
}

template <typename T>
ll_item<T>* linked_list<T>::unlink_item(llitem* unitem)
{
    if (!unitem || !head->get_next())
        return 0;
    if (unitem == head->get_next()) {
        llitem* tmp = unitem->get_next(); // 2nd?
        if (tmp == 0) { // only 1 item, remove..
            head->set_next(0);
            tail->set_prev(0);
            current = 0;
        }
        else {// 2nd item becomes 1st.
            head->set_next(tmp);
            tmp->set_prev(0);
        }
    }
    else if (unitem == tail->get_prev()) {
        llitem* tmp = unitem->get_prev();
        tmp->set_next(0); // penultimate becomes last
        tail->set_prev(tmp);
    }
    else {// somewhere between first and last
        llitem* tmp = unitem->get_next();
        tmp->set_prev(unitem->get_prev());
        tmp = unitem->get_prev();
        tmp->set_next(unitem->get_next());
    }
    // if we've just removed item that current is (which goto_
    // funcs use), reset current to first (if any).
    if (current == unitem) {
        if (unitem->get_prev())
            current = unitem->get_prev();
        else
            current = unitem->get_next();
    }
    count--;
    return unitem;
}

template <typename T>
ll_item<T>* linked_list<T>::find_data(llitem* from, T* data)
{
    if (!data || !head->get_next() || !from)
        return 0;
    llitem* tmp = from;
    while (tmp) {
        if (tmp->get_data() == data)
            return tmp;
        tmp = tmp->get_next();
    }
    return 0;
}

#endif
