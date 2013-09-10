#ifndef CONNECTORLIST_H
#include "../include/connectorlist.h"
#include "../include/synthmod.h"
#include "../include/jwm_globals.h"
#include "../include/listwork.h"

#include <iostream>

connectorlist::connectorlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
{
}

connectorlist::~connectorlist()
{
}

connector*
connectorlist::get_connector_by_input(synthmod* sm,
                                      inputnames::IN_TYPE it)
{
    if (!sm)
        return 0;
    connector* connect = goto_first();
    while(connect) {
        if (connect->get_input_module() == sm
            && connect->get_input_type() == it)
            return connect;
        connect = goto_next();
    }
    return 0;
}

connector* connectorlist::add_connector(connector* c)
{
    if (!c)
        return 0;
    return add_at_head(c)->get_data();
}

bool connectorlist::delete_connector(connector* c)
{
    if (!c)
        return false;
    llitem* tmp = find_data(sneak_first(), c);
    if (!tmp)
        return false;
    delete unlink_item(tmp);
    delete c;
    return true;
}

bool
connectorlist::duplicate_connections(
    synthmod* from_mod, synthmod* to_mod)
{
    if (from_mod->get_module_type() != to_mod->get_module_type())
        return false;
    llitem* i = find_in_data(sneak_first(), input_module(from_mod));
    while(i){
        add_connector(i->get_data()->duplicate(to_mod));
        i = find_in_data(i->get_next(), input_module(from_mod));
    }
    return true;
}


connectorlist::linkedlist*
connectorlist::duplicate_connections_for_module(
    synthmod* from_mod, synthmod* to_mod)
{
    if (from_mod->get_module_type() != to_mod->get_module_type())
        return 0;
    connectorlist::linkedlist*
        conlist = new linkedlist(MULTIREF_OFF, PRESERVE_DATA);
    llitem* i = find_in_data(sneak_first(), input_module(from_mod));
    while(i){
        conlist->add_at_head(i->get_data()->duplicate(to_mod));
        i = find_in_data(i->get_next(), input_module(from_mod));
    }
    return conlist;
}


connector*
connectorlist::duplicate_connections_for_group(
    char const* from, char const* to)
{
/*
    synthmodlist::linkedlist* fromlist =
        new_list_of_by(jwm.get_modlist(), from);

    synthmodlist::linkedlist* tolist =
        new_list_of_by(jwm.get_modlist(), to);

    if (!grpmodlist)
        return 0;

    synthmod* mod = fromlist->goto_first();
    synthmod* to_mod = tolist->goto_first();
    while(mod) {
        connectorlist*
            conlist = duplicate_connections_for_module(mod, to_mod);
        connector* con = conlist->goto_first();
    }
*/
    return 0;
}

void
connectorlist::reconnect_output_module_by_name(
    const char* from, const char* to)
{
    char spaces[50];
    for (int i = 0; i < 50; spaces[i] = ' ', i++);
    std::string cmsg;
    connector* connect = goto_first();
    while(connect) {
        if (strcmp(connect->get_output_module_name(), from) == 0) {
            connect->set_output_module_name(to);
            if (jwm.is_verbose()) {
                cmsg = "\nreforming connection: ";
                cmsg += connect->get_input_module()->get_username();
                cmsg += " ";
                cmsg += jwm.get_inputnames()->get_name(
                    connect->get_input_type());
                int i = cmsg.length();
                if (i > 40) i = 40;
                cmsg.append(spaces, 40 - i);
                std::cout << cmsg << "<-- ";
                std::cout << "from " << from << " to " << to << " ";
                std::cout << jwm.get_outputnames()->get_name(
                    connect->get_output_type());
            }
        }
        connect = goto_next();
    }
}

bool connectorlist::make_connections()
{
    char spaces[40];
    for (int i = 0; i < 40; spaces[i] = ' ', i++);
    std::string cmsg;
    connector* connect = goto_first();
    while(connect){
        if (!connect->connect())
            return false;
        if (jwm.is_verbose()) {
            cmsg = connect->get_output_module_name();
            cmsg += " ";
            cmsg += jwm.get_outputnames()->get_name(
                connect->get_output_type());
            int i = cmsg.length();
            if (i > 30) i = 30;
            cmsg.append(spaces, 30 - i);
            std::cout << "\n" << cmsg << "-->  ";
            std::cout << connect->get_input_module()->get_username();
            std::cout << " " << jwm.get_inputnames()->get_name(
                connect->get_input_type());
        }
        connect = goto_next();
    }
    std::cout << std::endl;
    return true;
}

#endif
