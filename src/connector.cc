#ifndef CONNECTOR_H
#include "../include/connector.h"

#ifndef BARE_MODULES

connector::connector(
 synthmod* input_module, inputnames::IN_TYPE input_type,
 char const* output_module_name, outputnames::OUT_TYPE output_type) : 
  in_mod(input_module), in_type(input_type), out_mod_uname(0), 
  out_type(output_type)
{
    out_mod_uname = new char[strlen(output_module_name) + 1];
    strcpy(out_mod_uname, output_module_name);
}

connector::~connector()
{
    delete [] out_mod_uname;
}

bool connector::connect()
{
    if (!this) {
        connect_err_msg =
            "\nCannot make connection, connection does not exist!";
        return false;
    }
    synthmod* outmod =
        synthmod::get_modlist()->get_synthmod_by_name(out_mod_uname);
    if (!outmod) {
        connect_err_msg = "\nIn module ";
        connect_err_msg += in_mod->get_username();
        connect_err_msg += ", cannot connect input ";
        connect_err_msg += synthmod::get_inputnames()->get_name(in_type);
        connect_err_msg += ", the module ";
        connect_err_msg += out_mod_uname;
        connect_err_msg += " does not exist";
        return false;
    }
    const void* out_data = outmod->get_out(out_type);
    if (!out_data) {
        connect_err_msg = "\nIn module ";
        connect_err_msg += in_mod->get_username();
        connect_err_msg += ", cannot connect input ";
        connect_err_msg += synthmod::get_inputnames()->get_name(in_type);
        connect_err_msg += ", to module ";
        connect_err_msg += out_mod_uname;
        connect_err_msg += ", it don't have any ";
        connect_err_msg 
         += synthmod::get_outputnames()->get_name(out_type);
        connect_err_msg += " output";
        return false;
    }
/*  this error will have been caught when creating the module
    if (synthmod::get_inputnames()->get_category(in_type) !=
            synthmod::get_outputnames()->get_category(out_type))
    {
        connect_err_msg = "\nCannot make connection, input and output\
         categories do not match:\ninput: ";
        connect_err_msg += in_mod->get_username();
        connect_err_msg += synthmod::get_inputnames()->get_name(in_type);
        connect_err_msg += "\noutput: ";
        connect_err_msg += out_mod_uname;
        connect_err_msg 
         += synthmod::get_outputnames()->get_name(out_type);
        return false;
    }
*/
    if (!in_mod->set_in(in_type, out_data)) {
        connect_err_msg = "\n*** MODULE ERROR ***";
        connect_err_msg += "\nIn module ";
        connect_err_msg += *in_mod->get_username();
        connect_err_msg += " not programmed to set input ";
        connect_err_msg += synthmod::get_inputnames()->get_name(in_type);
        return false;
    }
    return true;
}

string connector::connect_err_msg;

#endif
#endif
