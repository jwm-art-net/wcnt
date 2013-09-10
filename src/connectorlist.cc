#ifndef CONNECTORLIST_H
#include "../include/connectorlist.h"
#include <iostream>

#ifndef BARE_MODULES

connectorlist::connectorlist()
: connectlist(0), connect_item(0), connect(0)
{
	connectlist = new 
		linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

connectorlist::~connectorlist()
{
	goto_first();
	while(connect) {
		delete connect;
		goto_next();
	}
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
	outputnames* outnames = synthmod::get_outputnames();
	inputnames* innames = synthmod::get_inputnames();
	goto_first();
	do {
		// ok if list is empty cos it checks for that kind of thing:
		if (!connect->connect())
			return false;
		if (verbose) {
			cout << "\nmade connection: ";
			cout << connect->get_output_module_name();
			cout << " " << outnames->getname(connect->get_output_type());
			cout << "\t-->\t" << *(connect->get_input_module()->get_username());
			cout << " " << innames->getname(connect->get_input_type());
		}
		goto_next();
	}while(connect);
	return true;
}

#endif
#endif
