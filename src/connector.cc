#include "../include/connector.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/jwm_globals.h"

connector::connector(
 synthmod* input_module, inputnames::IN_TYPE input_type,
 const char* output_module_name, outputnames::OUT_TYPE output_type) :
  in_mod(input_module), in_type(input_type), out_mod_uname(0),
  out_type(output_type)
{
    set_output_module_name(output_module_name);
}

connector::~connector()
{
    delete [] out_mod_uname;
}

void connector::set_output_module_name(const char* output_module_name)
{
    if (out_mod_uname)
        delete [] out_mod_uname;
    out_mod_uname = new char[strlen(output_module_name) + 1];
    strcpy(out_mod_uname, output_module_name);
}

connector* connector::duplicate()
{
    return new connector(in_mod, in_type, out_mod_uname, out_type);
}

connector* connector::duplicate(synthmod* sm)
{
    if (in_mod->get_module_type() == sm->get_module_type())
        return new connector(sm, in_type, out_mod_uname, out_type);
    return 0;
}
#include <iostream>
bool connector::connect()
{
    if (!this) {
        connect_err_msg =
            "\nCannot make connection, connection does not exist!";
        return false;
    }
    const synthmod* outmod =
        jwm.get_modlist()->get_synthmod_by_name(out_mod_uname);
    if (!in_mod) {
        connect_err_msg = "\nConnection error! Bad News!";
        connect_err_msg += "\ninput module not set, nothing to connect";
        connect_err_msg += "to!";
        connect_err_msg += "\nFOI input type is set as ";
        connect_err_msg += jwm.get_inputnames()->get_name(in_type);
        connect_err_msg += " and out module name is ";
        connect_err_msg += out_mod_uname;
        return false;
    }
    if (!outmod) {
        connect_err_msg = "\nIn module ";
        connect_err_msg += in_mod->get_username();
        connect_err_msg += ", cannot connect input ";
        connect_err_msg += jwm.get_inputnames()->get_name(in_type);
        connect_err_msg += ", the module ";
        connect_err_msg += out_mod_uname;
        connect_err_msg += " does not exist";
        return false;
    }
    const void* const out_data = outmod->get_out(out_type);
//    std::cout << "\noutmod = " << out_mod_uname 
//        << " out_data = " << out_data;
    if (!out_data) {
        connect_err_msg = "\nIn module ";
        connect_err_msg += in_mod->get_username();
        connect_err_msg += ", cannot connect input ";
        connect_err_msg += jwm.get_inputnames()->get_name(in_type);
        connect_err_msg += ", to module ";
        connect_err_msg += out_mod_uname;
        connect_err_msg += ", it does not have any ";
        connect_err_msg += jwm.get_outputnames()->get_name(out_type);
        connect_err_msg += " output";
        return false;
    }
    if (!in_mod->set_in(in_type, out_data)) {
        connect_err_msg = "\n*** MODULE ERROR ***";
        connect_err_msg += "\nIn module ";
        connect_err_msg += *in_mod->get_username();
        connect_err_msg += " not programmed to set input ";
        connect_err_msg += jwm.get_inputnames()->get_name(in_type);
        return false;
    }
    return true;
}

std::string connector::connect_err_msg;

