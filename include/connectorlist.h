#ifndef CONNECTORLIST_H
#define CONNECTORLIST_H

#include "linkedlist.h"
#include "connector.h"

// make_connections() returns true if it all went as planned,
// returns false if any problem occured.  
// use connector::get_connect_err_msg() to let the user know
// what it got the hump with....

class connectorlist
{
 public:
	connectorlist();
	~connectorlist();
	connector* goto_first(){ return connect = (connector*)(connect_item = connectlist->goto_first())->get_data();}
	connector* goto_last(){ return connect = (connector*)(connect_item = connectlist->goto_last())->get_data();}
	connector* goto_prev(){ return connect = (connector*)(connect_item = connectlist->goto_prev())->get_data();}
	connector* goto_next(){ return connect = (connector*)(connect_item = connectlist->goto_next())->get_data();}
	connector* goto_connector(connector* rd){ return connect = (connector*)(connect_item = connectlist->find_data(rd))->get_data();}
	connector* add_connector(connector* rd);
	bool delete_connector(connector*);
	bool make_connections();
	void set_verbose(){verbose = true;}
	bool is_verbose(){return verbose;}
	
 private:
	linkedlist* connectlist;
	ll_item* connect_item;
	connector* connect;
	bool verbose;
};

#endif
