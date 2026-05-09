#include "../include/connector.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/globals.h"


#include <iostream>


#ifdef DEBUG
#define connerr(fmt, ... )                              \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(connector::err_msg, STRBUFLEN, fmt, __VA_ARGS__);   \
}
#else
#define connerr(fmt, ... ) \
    cfmt(connector::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif


connector::connector(
 synthmod::base* input_module, input::TYPE input_type,
 const char* output_module_name, output::TYPE output_type)
: in_mod(input_module), in_type(input_type), out_mod_uname(0),
  out_type(output_type), flags(0)
{
    set_output_module_name(output_module_name);
    #ifdef DATA_STATS
    STATS_INC
    #endif
}

connector::~connector()
{
    delete [] out_mod_uname;
    #ifdef DATA_STATS
    STATS_DEC
    #endif
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

connector* connector::duplicate(synthmod::base* sm)
{
    if (in_mod->get_module_type() == sm->get_module_type())
        return new connector(sm, in_type, out_mod_uname, out_type);
    return 0;
}

connector::CSTATE connector::connect()
{
    const synthmod::base* outmod =
        wcnt::jwm.get_modlist()->get_synthmod_by_name(out_mod_uname);
    if (!in_mod) {
        connerr("Connection error! Input module not set, nothing to "
                "connect to. FYI input type is set as %s and out module "
                "name is %s.", input::names::get(in_type),
                                                        out_mod_uname);
        return FAIL;
    }

    if (!outmod) {
        connerr("In module %s cannot connect input %s, module %s does "
                "not exist.", in_mod->get_username(),
                input::names::get(in_type), out_mod_uname);
        return FAIL;
    }
    const void* const out_data = outmod->get_out(out_type);

    if (!out_data) {
        if (outmod->flag(synthmod::base::SM_PASSTHROUGH)) {
            std::cout << "\nconnector::connect - postponing connection" << std::endl;
            return POSTPONE;
        }

        connerr("In module %s cannot connect input %s. Module %s does not "
                "have any %s output.", in_mod->get_username(),
                input::names::get(in_type), out_mod_uname,
                output::names::get(out_type));
        return FAIL;
    }
    if (!in_mod->set_in(in_type, out_data)) {
        connerr("*** MODULE ERROR *** In module %s not programmed to "
                "accept connections for input %s.", in_mod->get_username(),
                                                input::names::get(in_type));
        return FAIL;
    }
    return SUCCESS;
}

char connector::err_msg[STRBUFLEN];

#ifdef DATA_STATS
STATS_INIT(connector)
#endif

