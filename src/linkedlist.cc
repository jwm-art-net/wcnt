#ifndef LINKEDLIST_H
#include "../include/linkedlist.h"

ll_item::ll_item(void *d) :
 data(d), prev(0), next(0)
{
}

ll_item::~ll_item() 
{
}

linkedlist::linkedlist() :  
 head(0), tail(0), current(0), find_result(0),
 multiref(MULTIREF_OFF), nulldata(NO_NULLDATA)
{
    head = new ll_item(0);
    tail = new ll_item(0);
}

linkedlist::linkedlist(MULTIREF mf, NULLDATA nd) :
 head(0), tail(0), current(0), find_result(0), 
 multiref(mf), nulldata(nd)
{
    head = new ll_item(0);
    tail = new ll_item(0);
}

linkedlist::~linkedlist()
{
    ll_item *tmp;
    current = head->get_next();
    while (current) {
        tmp = current;
        current = current->get_next();
        delete tmp;
    }
    delete head;
    delete tail;
}

ll_item * linkedlist::add_at_head(void *data)
{
    if (!this)
        return 0;
    if (!data && nulldata == NO_NULLDATA)
        return 0;
    ll_item *newitem;
    if (!head->get_next()) {    /* is_empty() */
        newitem = new ll_item(data);
        head->set_next(newitem);
        newitem->set_prev(0);
        newitem->set_next(0);
        tail->set_prev(newitem);
    } else {
        ll_item *tmp;
        if (multiref == MULTIREF_OFF) {
            if (find_data(data))
                return 0;
        }
        newitem = new ll_item(data);
        tmp = head->get_next();
        newitem->set_next(tmp);
        newitem->set_prev(0);
        tmp->set_prev(newitem);
        head->set_next(newitem);
    }
    return current = newitem;
}

ll_item * linkedlist::add_at_tail(void *data)
{
    if (!this)
        return 0;
    if (!data && nulldata == NO_NULLDATA)
        return 0;
    ll_item *newitem;
    if (!head->get_next()) {    /* is_empty() */
        newitem = new ll_item(data);
        tail->set_prev(newitem);
        newitem->set_prev(0);
        newitem->set_next(0);
        head->set_next(newitem);
    } else {
        ll_item *tmp;
        if (multiref == MULTIREF_OFF) {
            if (find_data(data))
                return 0;
        }
        newitem = new ll_item(data);
        tmp = tail->get_prev();
        newitem->set_prev(tmp);
        newitem->set_next(0);
        tmp->set_next(newitem);
        tail->set_prev(newitem);
    }
    return current = newitem;
}

ll_item * linkedlist::insert_after(ll_item * llitem, void *data)
{
    if (!this)
        return 0;
    if (!llitem)
        return 0;
    if (!data && nulldata == NO_NULLDATA)
        return 0;
    if (!head->get_next())
    // empty list - llitem is not part of this list so cannot add after it
        return 0;  
    if (multiref == MULTIREF_OFF) {
        if (find_data(data))
            return 0;
    }
    ll_item *i;
    for (i = head->get_next(); i != 0; i = i->get_next()) {
        ll_item *newitem;
        if (i == llitem) {
            if (i == tail->get_prev()) {
                newitem = new ll_item(data);
                newitem->set_prev(i);
                newitem->set_next(0);
                i->set_next(newitem);
                tail->set_prev(newitem);
            } else {
                newitem = new ll_item(data);
                ll_item *j = i->get_next();
                i->set_next(newitem);
                newitem->set_prev(i);
                newitem->set_next(j);
                j->set_prev(newitem);
            }
            return current = newitem;
        }
    }
    return 0;
}

ll_item * linkedlist::unlink_item(ll_item * llitem)
{
    if (!this)
        return 0;
    if (!llitem || !head->get_next())
        return 0;
    ll_item *i;
    for (i = head->get_next(); i != 0; i = i->get_next()) {
        if (i == llitem) {
            if (i == head->get_next()) {
                ll_item *tmp = i->get_next();
                if (tmp == 0) {
                    head->set_next(0);
                    tail->set_prev(0);
                    current = 0;
                } else {
                    head->set_next(tmp);
                    tmp->set_prev(0);
                }
            } else if (i == tail->get_prev()) {
                ll_item *tmp = i->get_prev();
                tmp->set_next(0);
                tail->set_prev(tmp);
            } else {
                llitem = i->get_next();
                llitem->set_prev(i->get_prev());
                llitem = i->get_prev();
                llitem->set_next(i->get_next());
            }
            if (current == i)
                current = head->get_next();
            return i;
        }
    }
    return 0;
}

ll_item * linkedlist::goto_item(ll_item * llitem)
{
    if (!this)
        return 0;
    if (!llitem || !head->get_next())
        return 0;
    ll_item *i;
    for (i = head->get_next(); i != 0; i = i->get_next())
        if (i == llitem)
            return current = i;
    return current = 0;
}

ll_item * linkedlist::find_data(void *dt)
{
    if (!this)
        return 0;
    if (!dt || !head->get_next())
        return find_result = 0;
    ll_item *tmp = head->get_next();
    while (tmp) {
        if (tmp->get_data() == dt)
            return find_result = tmp;
        tmp = tmp->get_next();
    }
    return find_result = 0;
}

ll_item * linkedlist::find_next()
{
    if (!this)
        return 0;
    if (!find_result || !head->get_next())      /* is_empty() */
        return find_result = 0;
    void *fd = find_result->get_data();
    ll_item *tmp = find_result->get_next();
    while (tmp) {
        if (tmp->get_data() == fd)
            return find_result = tmp;
        tmp = tmp->get_next();
    }
    return find_result = 0;
}


bool linkedlist::is_first()
{
    if (!this)
        return 0;
    if (!head->get_next())      /* is_empty() */
        return 0;
    return (current == head->get_next());
}

bool linkedlist::is_last()
{
    if (!this)
        return 0;
    if (!head->get_next())      /* is_empty() */
        return 0;
    return (current == tail->get_prev());
}

int
linkedlist::get_item_count()
{
    if (!this)
        return 0;
    ll_item *i = head->get_next();
    int count = 0;
    while (i) {
        count++;
        i = i->get_next();
    }
    return count;
}

#endif
