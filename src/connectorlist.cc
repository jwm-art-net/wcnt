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
    return add_at_head(c)->get_data();
}

connector* connectorlist::add_connector_off(synthmod::base* sm, input::TYPE it)
{
    return add_connector(sm, it, "off", output::names::get_off_type(
                                                input::names::category(it)));
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
        if (!add_at_head(c)) {
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
        conlist->add_at_head(i->get_data()->duplicate(to_mod));
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
    while(connect){
        if (!connect->connect())
            return false;
        if (wcnt::jwm.is_verbose()) {
            const char* spc = spaces::get(40);
            cmsg = connect->get_output_module_name();
            cmsg += " ";
            cmsg += output::names::get(
                connect->get_output_type());
            int i = cmsg.length();
            if (i > 30)
                i = 30;
            cmsg.append(spc, 30 - i);
            std::cout << "\n" << cmsg << "-->  ";
            std::cout << connect->get_input_module()->get_username();
            std::cout << " " << input::names::get(
                                        connect->get_input_type());
        }
        connect = goto_next();
    }
    std::cout << std::endl;
    return true;
}

#ifdef UNUSED
connector*
connectorlist::duplicate_connections_for_group(
    const char* from, const char* to)
{
    /* FIXME: "nothing here codes be doing.." */
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

bool
connectorlist::remake_connections(
    synthmod* sm, output::TYPE ot, output::TYPE new_ot)
{
    const char* smname = sm->get_username();
    llitem* i = find_in_data(sneak_first(), name(smname));
    if (!i)
        return false;
    if (wcnt::jwm.is_verbose())
        std::cout << "\nRemaking connections for " << smname;
    while(i){
        connector* connect = i->get_data();
        if (connect->get_output_type() == ot) {
            connect->set_output_type(new_ot);
            if (!connect->connect()) {
                return false;
            }
        }
        i = find_in_data(i->get_next(), name(smname));
    }
    return true;
}
#endif
