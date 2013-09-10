#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "synthmodulelist.h"

/*
    duplicate() duplicates the connector exactly and returns the new
    connector.
    duplicate(synthmod*) checks the synthmod is of the same type and
    recreates the connector using the synthmod passed. returns 0 when
    the check fails.
*/

class connector
{
public:
    connector(synthmod* input_module,
              inputnames::IN_TYPE input_type,
              char const* output_module_name,
              outputnames::OUT_TYPE output_type );
    ~connector();
    bool set_input_module(synthmod*);
    synthmod* get_input_module(){ return in_mod;}
    inputnames::IN_TYPE get_input_type(){ return in_type;}
    void set_output_module_name(char const*);
    char const* get_output_module_name(){ return out_mod_uname;}
    outputnames::OUT_TYPE get_output_type(){ return out_type;}
    // only do this once all synth modules have been created.
    // otherwise modules may reference as yet uncreated modules.
    // connect() would start crying if that happend.
    bool connect();
    connector* duplicate();
    connector* duplicate(synthmod*);
    static string const* get_connect_err_msg(){ return &connect_err_msg;}
private:
    synthmod* in_mod;
    inputnames::IN_TYPE in_type;
    char* out_mod_uname;
    outputnames::OUT_TYPE out_type;
    static string connect_err_msg;
};
#endif
