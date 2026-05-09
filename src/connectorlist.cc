#include "../include/connectorlist.h"
#include "../include/synthmod.h"
#include "../include/globals.h"
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
connectorlist::get_connector_by_input(const synthmod::base* sm,
                                                input::TYPE it)
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
    return add_at_tail(c)->get_data();
}

connector* connectorlist::add_connector_off(synthmod::base* sm, input::TYPE it)
{
    return add_connector(sm, it, "off",
                            output::names::get_off_type(
                                input::names::get_off_category_for_input(it)));
}

connector* connectorlist::add_connector_self(synthmod::base* sm, input::TYPE it,
                                                                 output::TYPE ot)
{
    return add_connector(sm, it, sm->get_username(), ot);
}


connector* connectorlist::add_connector_as(synthmod::base* sm, input::TYPE it,
                                                               input::TYPE itas)
{
    llitem* li = find_in_data(sneak_first(), input_module(sm));
    while (li) {
        connector* c = li->get_data();
        if (c->get_input_type() == itas)
            return add_connector(sm, it, c->get_output_module_name(), c->get_output_type());
        li = find_in_data(li->get_next(), input_module(sm));
    }

    return 0;
}

connector* connectorlist::add_connector(synthmod::base* sm, input::TYPE it,
                                        const char* out_mod, output::TYPE ot)
{
    connector* c = new connector(sm, it, out_mod, ot);
    if (c) {
        if (!add_at_tail(c)) {
            delete c;
            c = 0;
        }
    }
    return c;
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
connectorlist::duplicate_connections(const synthmod::base* from_mod,
                                           synthmod::base* to_mod)
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
                const synthmod::base* from_mod, synthmod::base* to_mod)
{
    if (from_mod->get_module_type() != to_mod->get_module_type())
        return 0;
    connectorlist::linkedlist*
        conlist = new linkedlist(MULTIREF_OFF, PRESERVE_DATA);
    llitem* i = find_in_data(sneak_first(), input_module(from_mod));
    while(i){
        conlist->add_at_tail(i->get_data()->duplicate(to_mod));
        i = find_in_data(i->get_next(), input_module(from_mod));
    }
    return conlist;
}

void
connectorlist::reconnect_output_module_by_name(
    const char* from, const char* to)
{
    const char* spc = spaces::get(40);
    std::string cmsg;
    connector* connect = goto_first();
    while(connect) {
        if (strcmp(connect->get_output_module_name(), from) == 0) {
            connect->set_output_module_name(to);
            if (wcnt::jwm.is_verbose()) {
                cmsg = "\nreforming connection: ";
                cmsg += connect->get_input_module()->get_username();
                cmsg += " ";
                cmsg += input::names::get(connect->get_input_type());
                int i = cmsg.length();
                if (i > 40) i = 40;
                cmsg.append(spc, 40 - i);
                std::cout << cmsg << "<-- ";
                std::cout << "from " << from << " to " << to << " ";
                std::cout << output::names::get(
                    connect->get_output_type());
            }
        }
        connect = goto_next();
    }
}

bool connectorlist::make_connections()
{
    std::string cmsg;
    connector* connect = goto_first();
    llitem* lastitem = sneak_last();
    llitem* item = sneak_current();
    int postponed_connection_level = 0;

    while(connect){
        llitem* nextitem = 0;
        // build informative connection text for user
        const char* spc = spaces::get(40);
        cmsg = connect->get_output_module_name();
        cmsg += " ";
        cmsg += output::names::get(
            connect->get_output_type());
        int i = cmsg.length();
        if (i > 30)
            i = 30;
        cmsg.append(spc, 30 - i);
        cmsg += "-->  ";
        cmsg += connect->get_input_module()->get_username();
        cmsg += " ";
        cmsg += input::names::get(connect->get_input_type());

        switch(connect->connect())
        {
            case connector::SUCCESS:
                if (wcnt::jwm.is_verbose()) {
                    std::cout << cmsg << std::endl;
                }
                break;

            case connector::POSTPONE:
                if (postponed_connection_level > wcnt::max_connection_postponement_level) {
                    // FIXME: look into better error handling for wcnt, there's no error handling for
                    // the connectorlist itself, only for the connect class
                    std::cout << "Connection postponement failed - maximum connection postponement level "
                              << wcnt::max_connection_postponement_level << " exceeded!" << std::endl;
                    return false;
                }
                if (wcnt::jwm.is_verbose()) {
                    std::cout << "Postponing connection " << cmsg << std::endl;
                }

                // need to keep track of next item
                nextitem = sneak_next();
                if (!nextitem) {
                    // FIXME: error handling (see above).
                    std::cout << "connection postponed, but no other connections remain." << std::endl;
                    return false;
                }
                item = sneak_current();
                unlink_item(item);
                add_at_tail(item->get_data());
                // make current point back to what was next
                break;
            default:
                // FIXME: error handling (see above).
                return false;
        }
        if (sneak_current() == lastitem && lastitem != sneak_last()) {
            postponed_connection_level++;
            #ifdef DEBUG
            std::cout << "Beginning postponed connections processing... level:" << postponed_connection_level << std::endl;
            #endif
            lastitem = sneak_last();
        }
        if (!nextitem)
            connect = goto_next();
        else
            connect = goto_item(nextitem);
    }
    return true;
}

