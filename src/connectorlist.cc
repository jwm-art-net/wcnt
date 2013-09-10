#ifndef CONNECTORLIST_H
#include "../include/connectorlist.h"

connectorlist::connectorlist() :
 connectlist(0), connect_item(0), connect(0), verbose(false)
{
    connectlist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

connectorlist::~connectorlist()
{
    goto_first();
    while(connect) {
        delete connect;
        goto_next();
    }
    delete connectlist;
}

connector* 
connectorlist::get_connector_by_input(
    synthmod* sm, inputnames::IN_TYPE it)
{
    if (!sm)
        return 0;
    goto_first();
    while(connect) {
        if (connect->get_input_module() == sm
            && connect->get_input_type() == it) return connect;
        goto_next();
    }
    return 0;
}

connector* connectorlist::add_connector(connector* c)
{
    if (!c)
        return 0;
    return connect = (connector*)
     (connect_item = connectlist->add_at_head(c))->get_data();
}

bool connectorlist::delete_connector(connector* c)
{
    if (!c)
        return false;
    ll_item* tmp = connectlist->find_data(c);
    if (!tmp)
        return false;
    if (tmp == connect_item)
        goto_first();
    delete connectlist->unlink_item(tmp);
    delete c;
    return true;
}

bool connectorlist::make_connections()
{
    char spaces[40];
    int i;
    for (i = 0; i < 40; i++) 
        spaces[i] = ' ';
    string cmsg;
    outputnames* outnames = synthmod::get_outputnames();
    inputnames* innames = synthmod::get_inputnames();
    goto_first();
    do {
        // ok if list is empty cos it checks for that kind of thing:
        if (!connect->connect())
            return false;
        if (verbose) {
            cmsg = connect->get_output_module_name();
            cmsg += " ";
            cmsg += outnames->get_name(connect->get_output_type());
            i = cmsg.length();
            cmsg.append(spaces, 30 - i);
            cout << "\n" << cmsg << "-->  ";
            cout << connect->get_input_module()->get_username();
            cout << " " << innames->get_name(connect->get_input_type());
        }
        goto_next();
    }while(connect);
    cout << endl;
    return true;
}

#endif
