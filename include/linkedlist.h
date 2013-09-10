#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*
	Written by James William Morris : sirrom75@hotmail.com
	see also www.britishpaintingforsale.com/James%20Morris.asp

	DESCRIPTION
	-----------
		class ll_item: 		an element in the linkedlist
		class linkedlist:		a doubly linked list of ll_items.  
		
	MULTIPLE REFERENCES to a single data item
	-----------------------------------------
		default behaviour cannot have several ll_items pointing to one 
		peice of data. Any atempts to do so will fail and return NULL.
		the user will need to check this.
		
		To allow multiple references to the same data item use MULTIREF_ON
		in the linkedlist constructor.  You cannot change this behaviour
		once the linkedlist is constructed.  
		
	NULLDATA
	--------
		The default behaviour rejects ll_items whose data element is NULL
		Use ALLOW_NULLDATA in constructor to change this.  You cannot change
		this behaviour once the linkedlist is constructed.
		
	HEAD and TAIL
	-------------
		head and tail members never hold any data, except for next and prev
		members which point to the first and last elements in the list. these 
		members remain private and hidden to 3rd parties. The first element's
		prev member allways points to NULL, as does the last element's next 
		member.
		
	SEARCHING
	---------
		The find_data(void* data) member function searches the linkedlist 
		until it finds the ll_item which points to void* data.  find_data()
		stores the ll_item found, for find_next() to know where to start
		searching, and to know what to look for.  find_next() is only usefull
		if linkedlist constructor was called with MULTIREF_ON.
		
	DESTRUCTOR
	----------
		ll_item destructor does not delete the object pointed to by its data 
		member.  linkedlist, likewise does not delete this either.
		
	GENERAL DESIGN
	--------------
		I had initially intended for these classes to be inherited, but I've
		decided that they should not be.  
		
		I use it as follows:-
		
		the listed objects do not use ll_item whatsoever. the list object has 
		a private member of type linkedlist. the list object will have its own 
		member functions to access the linked list member functions, depending 
		on it's requirements.  These member functions will do the typecasting, 
		and provide the typecast saftey beloved by the sensible programmers of 
		the world!
		
		the list object's destructor will pass through the list and delete each
		listed object pointed to by ll_item::data but it will not delete ll_item.
		That job should be researved for the linkedlist destructor.
		
	NULL CHECKING
	-------------
		I've added checking for this==NULL to ll_item::get_????() functions, but
		not to ll_item::set_????() functions.  This is because I thought it better
		to get segfaults from trying use the set_????() functions on a NULL ll_item
		than not doing anything. In any case the user should check, and linkedlist
		code refuses to add a NULL ll_item to the list anyway.
		Checking for this==NULL in the get_????() functions is a whole lot more
		usefull as it saves extra checks elsewhere.
		
	INDEXING does not exist!
	------------------------
		there were methods such as goto_item_no(int), unlink_item_no(int)
		and add_item_to_be_no(ll_item*, int).  I have taken these out because
		of doubts about usefulness when I have not even bothered to overload any
		[] operators and I don't really need them for my purpose anyway.
		The implementation remains in linkedlist_no().h & linkedlist_no().cc
		
	LINKEDLIST RETURN VALUES
	------------------------
		add_at_head(ll_item*), and add_at_tail(ll_item*) return the ll_item added
		if nothing went wrong.  insert_item_after(...) returns NULL if the after 
		item does not exist in the list, else the ll_item added. unlink item returns
		null if the item does not exist in the list, else the item unlinked.
		
		goto_????() functions set current to, and return NULL if something goes 
		wrong. sneak_????() functions just return next or prev without setting 
		current.	find_????() functions do not set current either, but return result, 
		NULL, if not found.
	
	ADDED: 
	------------
		ORDERED_INSERT TEMPLATE FUNCTION
		-------------------------------------------------------------------
		this can be used with any object type which has a method which returns
		any type of value that can be used in comparisons in regards to order.
		see far below.
		
		LOOKUP_DATA_MATCH TEMPLATE FUNCTION
		---------------------------------------------------------------------------
		this can be used with any object type which has a method which returns
		any type of value that can be used in comparisons.  Use this to check for 
		a data item value within an object in a linkedlist.
		to search again use lookup_data_match_next. 
		NOTE: the lookup... template functions use set/get tempory so do not change
		tempory in between lookups if you want them to work properly.
		see far below.
*/

class ll_item
{
  public:
    ll_item(void *data);
     ~ll_item();
    bool set_prev(ll_item * p) {
        if (this) {
            prev = p;
            return true;
        }
        return false;
    }
    bool set_next(ll_item * n) {
        if (this) {
            next = n;
            return true;
        }
        return false;
    }
	void* set_data(void * d) { return (this) ? (data = d) : 0;}
    void* get_data() { return (this) ? data : 0; }
    ll_item *get_prev() { return (this) ? prev : 0; }
    ll_item *get_next() { return (this) ? next : 0; }

  private:
    void *data;
    ll_item *prev;
    ll_item *next;
};

class linkedlist
{
  public:
    enum MULTIREF
    { MULTIREF_OFF, MULTIREF_ON };
    enum NULLDATA
    { NO_NULLDATA, ALLOW_NULLDATA };
      linkedlist();
      linkedlist(MULTIREF, NULLDATA);
     ~linkedlist();
    ll_item *add_at_head(void *data);
    ll_item *add_at_tail(void *data);
    ll_item *insert_after(ll_item * llitem, void *data);
    ll_item *unlink_item(ll_item *);
    ll_item *goto_first() { return (this) ? current = head->get_next() : 0;}
    ll_item *goto_last()  { return (this) ? current = tail->get_prev() : 0;}
	ll_item *goto_prev()  { return (this) ? current = current->get_prev() : 0;}
    ll_item *goto_next()  { return (this) ? current = current->get_next() : 0;}                           /* see ll_item for proof */
    ll_item *sneak_first(){ return ((this) ? head->get_next() : 0);}
    ll_item *sneak_last() { return (this) ? tail->get_prev() : 0;}
    ll_item *sneak_prev() { return (this) ? current->get_prev() : 0;}
    ll_item *sneak_next() { return (this) ? current->get_next() : 0;}
    ll_item *sneak_current() { return (this) ? current : 0;}
    ll_item *goto_item(ll_item *);
    ll_item *find_data(void *);
    ll_item *find_next();
    ll_item *get_found() { return (this) ? find_result : 0;}
    void set_tempory(ll_item * tmp) { if (this) tempory = tmp;}
    ll_item *get_tempory() { return (this) ? tempory : 0;}
    bool is_first();
    bool is_last();
    bool is_empty() { return (this) ? (head->get_next() == 0) : false; }
    int get_item_count();
  private:
    ll_item * head;
    ll_item *tail;
    ll_item *current;
    ll_item *find_result;
    ll_item *tempory;
    MULTIREF multiref;
    NULLDATA nulldata;
};

/*
	ORDERED_INSERT FUNCTION
	-----------------------
		pass the linkedlist to insert to, the item to insert, and the function
		to use to get a numerical value from the object pointed to by the ll_items
		in the list.  the function must return a value that can be compared.
		this assumes that all ll_items in the list are all of the same class the
		pointed-to function comes from.
*/

template < typename T, typename R >
    T * ordered_insert(linkedlist * thelist, T * data, R(T::*order_func) ())
{
    if (!thelist || !data)
        return 0;
    if (thelist->is_empty())
        return (T *) thelist->add_at_head(data)->get_data();
    ll_item *tmp;
    T *tdata = (T *) (tmp = thelist->goto_first())->get_data();
    R newval = (data->*order_func) ();
    while (tmp) {
        if ((tdata->*order_func) () > newval) {
            ll_item *ptmp = tmp->get_prev();
            if (!ptmp)
                return (T *) thelist->add_at_head(data)->get_data();
            else
                return (T *) thelist->insert_after(ptmp, data);
        }
        tdata = (T *) (tmp = thelist->goto_next())->get_data();
    }
    return (T *) thelist->add_at_tail(data)->get_data();
}

/*
	LOOKUP_DATA_MATCH
	--------------------------------------
		pass the linkedlist to search through (thelist), the data object which contains the 
		value to check for (data), and the function which returns that value (match_func).
*/

template < typename T, typename R >
    T * lookup_data_match(linkedlist * thelist, T * data,
                          R(T::*match_func) ())
{
    if (!thelist || !data)
        return 0;
    if (thelist->is_empty())
        return 0;
    R match = (data->*match_func) ();
    ll_item *tmp;
    T *tdata = (T *) (tmp = thelist->goto_first())->get_data();
    while (tmp) {
        if ((tdata->*match_func) () == match) {
            thelist->set_tempory(tmp);  // store search result in list  
            return tdata;
        }
        tdata = (T *) (tmp = thelist->goto_next())->get_data();
    }
    thelist->set_tempory(0);
    return 0;
}

/*
	LOOKUP_DATA_MATCH_NEXT
	-------------------------------------------------
		more or less the same, but pass the data object returned from lookup_data_match
		ie an object already in the list to start the search from. (instead of an object outside
		of the list).
*/
template < typename T, typename R >
    T * lookup_data_match_next(linkedlist * thelist, T * data,
                               R(T::*match_func) ())
{
    if (!thelist || !data)
        return 0;
    if (thelist->is_empty())
        return 0;
    if ((thelist->get_tempory())->get_data() != data)
        return 0;               // data passed is not what was last found
    R match = (data->*match_func) ();
    ll_item *tmp;
    T *tdata = (T *) (tmp =
                      thelist->goto_item(thelist->get_tempory())->
                      goto_next())->get_data();
    while (tmp) {
        if ((tdata->*match_func) () == match) {
            thelist->set_tempory(tmp);  // store search result in list 
            return tdata;
        }
        tdata = (T *) (tmp = thelist->goto_next())->get_data();
    }
    thelist->set_tempory(0);
    return 0;
}

#endif
