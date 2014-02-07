#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "inputnames.h"
#include "function_objects.h"
#include "outputnames.h"
#include "types.h"

/*
//  duplicate() duplicates the connector exactly and returns the new
//  connector.
//  duplicate(synthmod::base*) checks the synthmod is of the same type and
//  recreates the connector using the synthmod passed. returns 0 when
//  the check fails.
*/

namespace synthmod { class base; }

class input_module
{
 public:
    input_module(synthmod::base* sm) : smod(sm) {};
    input_module(const synthmod::base* sm) : smod(sm) {};
    bool operator()(synthmod::base* sm) const { return smod == sm; }
    bool operator()(const synthmod::base* sm) const { return smod == sm; }

 private:
    const synthmod::base* smod;
};


class connector
{
 public:
    connector(synthmod::base* input_module,
              int input_type,
              const char* output_module_name,
              int output_type );
    ~connector();

    void set_output_module_name(const char*);
    void set_output_type(int ot){ out_type = ot; }

    const synthmod::base* get_input_module() const  { return in_mod;}
    int             get_input_type() const  { return in_type;}
    const char*     get_output_module_name() const  { return out_mod_uname;}
    int             get_output_type() const { return out_type;}
/*
//  only do this once all synth modules have been created.
//  otherwise modules may reference as yet uncreated modules.
//  connect() would start crying if that happend.
*/
    bool connect();
    connector* duplicate();
    connector* duplicate(synthmod::base*);
    static const char* get_connect_err_msg() { return err_msg;}

    bool operator()(input_module & inmod) { return (inmod(in_mod)); }
    bool operator()(fnobj::name & outname) { return (outname(out_mod_uname)); }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    synthmod::base* in_mod;
    int             in_type;
    char*           out_mod_uname;
    int             out_type;
    static char     err_msg[STRBUFLEN];

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};


#endif
