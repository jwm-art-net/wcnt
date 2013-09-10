#ifndef CONNECTORLIST_H
#include "../include/connectorlist.h"

connectorlist::connectorlist() :
 connectlist(0), connect_item(0), connect(0),
 delete_connections(true), verbose(false)
{
    connectlist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

connectorlist::connectorlist(DELCON) :
 connectlist(0), connect_item(0), connect(0),
 delete_connections(false), verbose(false)
{
    connectlist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

connectorlist::~connectorlist()
{
    goto_first();
    if (delete_connections) {
        while(connect) {
            delete connect;
        goto_next();
        }
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

connectorlist* connectorlist::duplicate_connections_for_module(
                synthmod* from_mod, synthmod* to_mod)
{
    connectorlist* conlist = new connectorlist(NO_DELETE_CONNECTIONS);
    if (from_mod->get_module_type() != to_mod->get_module_type())
        return conlist; // don't bother and return it empty.
    goto_first();
    while(connect) {
        if (connect->get_input_module() == from_mod)
            conlist->add_connector(connect->duplicate(to_mod));
        goto_next();
    }
    return conlist;
}

void connectorlist::reconnect_output_module_by_name(
                                        const char* from, const char* to)
{
    char spaces[50];
    for (int i = 0; i < 50; spaces[i] = ' ', i++);
    string cmsg;
    outputnames* outnames = synthmod::get_outputnames();
    inputnames* innames = synthmod::get_inputnames();
    goto_first();
    while(connect) {
        if (strcmp(connect->get_output_module_name(), from) == 0) {
            connect->set_output_module_name(to);
            if (verbose) {
                cmsg = "\nreforming connection: ";
                cmsg += connect->get_input_module()->get_username();
                cmsg += " ";
                cmsg += innames->get_name(connect->get_input_type());
                int i = cmsg.length();
                if (i > 40) i = 40;
                cmsg.append(spaces, 40 - i);
                cout << cmsg << "<-- ";
                cout << "from " << from << " to " << to << " ";
                cout << outnames->get_name(connect->get_output_type());
            }
        }
        goto_next();
    }
}

bool connectorlist::make_connections()
{
    char spaces[40];
    for (int i = 0; i < 40; spaces[i] = ' ', i++);
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
            int i = cmsg.length();
            if (i > 30) i = 30;
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
