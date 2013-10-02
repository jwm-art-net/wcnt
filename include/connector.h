#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "inputnames.h"
#include "namefuncobj.h"
#include "outputnames.h"
#include "types.h"

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
              input::TYPE input_type,
              const char* output_module_name,
              output::TYPE output_type );
    ~connector();

    void set_output_module_name(const char*);
    void set_output_type(output::TYPE ot){ out_type = ot; }

    const synthmod*       get_input_module()const { return in_mod;}
    input::TYPE   get_input_type()  const { return in_type;}
    const char* get_output_module_name()    const { return out_mod_uname;}
    output::TYPE get_output_type() const { return out_type;}
/*
//  only do this once all synth modules have been created.
//  otherwise modules may reference as yet uncreated modules.
//  connect() would start crying if that happend.
*/
    bool connect();
    connector* duplicate();
    connector* duplicate(synthmod*);
    static const char* get_connect_err_msg() { return err_msg;}

    bool operator()(input_module & inmod){
        return (inmod(in_mod));
    }

    bool operator()(name & outname){
        return (outname(out_mod_uname));
    }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod* in_mod;
    input::TYPE in_type;
    char* out_mod_uname;
    output::TYPE out_type;
    static char err_msg[STRBUFLEN];

    #ifdef DATA_STATS
    STATS_VARS
    #endif

};




#endif
