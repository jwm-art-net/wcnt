#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "inputnames.h"
#include "outputnames.h"

#include <string>

/*
//  duplicate() duplicates the connector exactly and returns the new
//  connector.
//  duplicate(synthmod*) checks the synthmod is of the same type and
//  recreates the connector using the synthmod passed. returns 0 when
//  the check fails.
*/

class synthmod;

class input_module
{
 public:
    input_module(synthmod* sm) : smod(sm) {};
    input_module(synthmod const* sm) : smod(sm) {};
    bool operator()(synthmod* sm) const { return smod == sm; }
    bool operator()(synthmod const* sm) const { return smod == sm; }
 private:
    synthmod const* smod;
};


class connector
{
public:
    connector(synthmod* input_module,
              inputnames::IN_TYPE input_type,
              const char* output_module_name,
              outputnames::OUT_TYPE output_type );
    ~connector();

    bool set_input_module(synthmod*);
    void set_output_module_name(const char*);

    const synthmod* const get_input_module()const { return in_mod;}
    inputnames::IN_TYPE   get_input_type()  const { return in_type;}
    char const* get_output_module_name()    const { return out_mod_uname;}
    outputnames::OUT_TYPE get_output_type() const { return out_type;}
/*
//  only do this once all synth modules have been created.
//  otherwise modules may reference as yet uncreated modules.
//  connect() would start crying if that happend.
*/
    bool connect();
    connector* duplicate();
    connector* duplicate(synthmod*);
    static const std::string* get_connect_err_msg()
        { return &connect_err_msg;}

    bool operator()(input_module & inmod){
        return (inmod(in_mod));
    }

private:
    synthmod* in_mod;
    inputnames::IN_TYPE in_type;
    char* out_mod_uname;
    outputnames::OUT_TYPE out_type;
    static std::string connect_err_msg;
};
#endif
