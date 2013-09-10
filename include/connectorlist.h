#ifndef CONNECTORLIST_H
#define CONNECTORLIST_H

#include <iostream>

#include "linkedlist.h"
#include "connector.h"

// make_connections() returns true if it all went as planned,
// returns false if any problem occured.
// use connector::get_connect_err_msg() to let the user know
// what it got the hump with....

/*
    the first constructor sets delete_connections to true,
    the second constructor sets delete_connections to false.
    connectorlist* duplicate_connections_for_module(synthmod* from,
    synthmod* to) creates a connectorlist using the second constructor.
    it is assumed that the connections will be added to a connectorlist
    created using the first constructor, (thus get deleted). if not you
    can call set_delete_connections() to cancel this behaviour and get
    them deleted by the destructor.
*/

class connectorlist
{
public:
    enum DELCON { NO_DELETE_CONNECTIONS };
    connectorlist();
    connectorlist(DELCON);
    ~connectorlist();
    connector* goto_first() {
        return connect = (connector*)
         (connect_item = connectlist->goto_first())->get_data();
    }
    connector* goto_last() {
        return connect = (connector*)
         (connect_item = connectlist->goto_last())->get_data();
    }
    connector* goto_prev() {
        return connect = (connector*)
         (connect_item = connectlist->goto_prev())->get_data();
    }
    connector* goto_next() {
        return connect = (connector*)
         (connect_item = connectlist->goto_next())->get_data();
    }
    connector* goto_connector(connector* rd) {
        return connect = (connector*)
         (connect_item = connectlist->find_data(rd))->get_data();
    }
    connector* get_connector_by_input(synthmod*, inputnames::IN_TYPE);
    connector* add_connector(connector* rd);
    bool delete_connector(connector*);
    connectorlist* duplicate_connections_for_module(synthmod* from,
                                                    synthmod* to);
    void reconnect_output_module_by_name(const char*from, const char*to);
    void set_delete_connections() { delete_connections = true; }
    bool make_connections();
    void set_verbose(){verbose = true;}
    bool is_verbose(){return verbose;}

private:
    linkedlist* connectlist;
    ll_item* connect_item;
    connector* connect;
    bool delete_connections;
    bool verbose;
};

#endif
