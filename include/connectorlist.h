#ifndef CONNECTORLIST_H
#define CONNECTORLIST_H

#include "linkedlist.h"
#include "connector.h"

/*
//  make_connections() returns true if it all went as planned,
//  returns false if any problem occured.
//  use connector::get_connect_err_msg() to let the user know
//  what it got the hump with....
//  the first constructor sets delete_connections to true,
//  the second constructor sets delete_connections to false.
//  connectorlist* duplicate_connections_for_module(synthmod::base* from,
//  synthmod::base* to) creates a connectorlist using the second constructor.
//  it is assumed that the connections will be added to a connectorlist
//  created using the first constructor, (thus get deleted). if not you
//  can call set_delete_connections() to cancel this behaviour and get
//  them deleted by the destructor.
*/

class connectorlist : public linked_list<connector>
{
public:
    connectorlist(){};
    connectorlist(DESTRUCTION);
    ~connectorlist();

    connector* get_connector_by_input(const synthmod::base*, input::TYPE);

    connector* add_connector(connector* rd);
    connector* add_connector_off(synthmod::base* sm, input::TYPE);
    connector* add_connector_self(synthmod::base* sm, input::TYPE,
                                                      output::TYPE);
    connector* add_connector_as(synthmod::base* sm, input::TYPE,
                                                    input::TYPE as);
    connector* add_connector(synthmod::base* sm,  input::TYPE,
                             const char* out_mod, output::TYPE);
    bool delete_connector(connector*);

    /*
    // duplicate_connections
    //  duplicates and adds the connections
    */
    bool duplicate_connections(const synthmod::base* from, synthmod::base* to);

    /*
    // duplicates_connections_for_module
    //  duplicates connections into a new list for further processing
    */
    linkedlist*
    duplicate_connections_for_module(const synthmod::base* from,
                                           synthmod::base* to);

    void reconnect_output_module_by_name(const char*from, const char*to);

    bool make_connections();

    #ifdef UNUSED
    /*
    // duplicate_connections_for_group
    //  returns 0 on sucess, or the connector which failed.
    */
    connector*
    duplicate_connections_for_group(const char* from, const char* to);

    // wcnt_signal uses output = input * level.
    // when level is 1.0, output == input, and the run method
    // is removed, and the output is redirected to become the input.
    // unfortunately, this is done via synthmod::init, which is called
    // after make_connections, so, remake_connections is called during
    // wcnt_signal::init to sort out the problem. other modules may
    // also utilise this method if possible.
    bool remake_connections(
        synthmod::base*, output::TYPE, output::TYPE new_ot);
    #endif
};

#endif
