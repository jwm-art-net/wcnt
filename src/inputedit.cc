#include "../include/inputedit.h"
#include "../include/connectorlist.h"
#include "../include/synthmodlist.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/dobjparamlist.h"

#include <iostream>
#include <sstream>



inputedit::inputedit() :
 dobj(dataobj::SIN_EDIT_INPUT),
 modname(0), iostr(0)
{
    init_first();
}

inputedit::~inputedit()
{
    if (modname)
        delete [] modname;
    if (iostr)
        delete [] iostr;
}

bool inputedit::set_modname(const char* n)
{
    synthmod* sm = jwm.get_modlist()->get_synthmod_by_name(n);
    if (!sm)
        return false;
    if (modname)
        delete [] modname;
    modname = new char[strlen(n) + 1];
    strcpy(modname, n);
    return true;
}

void inputedit::set_iostr(const char* n)
{
    if (iostr)
        delete [] iostr;
    iostr = new char[strlen(n) + 1];
    strcpy(iostr, n);
    return;
}

bool inputedit::create_connectors()
{
    synthmod* in_sm;
    synthmodlist* smlist = jwm.get_modlist();
    if (!(in_sm = smlist->get_synthmod_by_name(modname))) {
        dobjerr("Input module %s does not exist.", modname);
        invalidate();
        return false;
    }
    std::stringstream strm;
    strm << iostr;
    std::string input_name;
    strm >> input_name;
    do{
        input::TYPE in_type;
        in_type = input::names::type(input_name.c_str());
        if (in_type == input::ERR_TYPE) {
            dobjerr("Unrecognised input type %s, in connection for %s.",
                                            input_name.c_str(), modname);
            invalidate();
            return false;
        }
        std::string out_modname;
        std::string output_name;
        output::TYPE out_type;
        strm >> out_modname;
        if (strcmp(out_modname.c_str(), "off") == 0) {
            out_type = output::names::get_off_type(
                        input::names::category(in_type));
        }
        else {
            // allow output module to not exist yet.
            strm >> output_name;
            out_type = output::names::type(output_name.c_str());
            if (out_type == output::ERR_TYPE) {
                dobjerr("Unrecognised output type %s in connection for %s.",
                                            output_name.c_str(), modname);
                invalidate();
                return false;
            }
            if (input::names::category(in_type)
            != output::names::category(out_type))
            {
                dobjerr("In connection for %s output %s does not match "
                                "category of input %s.", modname,
                                output_name.c_str(), input_name.c_str());
                invalidate();
                return false;
            }
        }
        modinputlist::linkedlist* inlist =
            new_list_of_by(jwm.get_inputlist(), in_sm);
        modinput* sm_inp = inlist->goto_first();
        bool success = false;
        while(sm_inp && !success) {
            if (sm_inp->get_inputtype() == in_type) {
                connector* con;
                connectorlist* conlist = jwm.get_connectlist();
                while((con = conlist->
                    get_connector_by_input(in_sm, in_type)))
                {
                    conlist->delete_connector(con);
                }
                con = new connector(in_sm, in_type,
                                    out_modname.c_str(), out_type);
                conlist->add_connector(con);
                success = true;
            }
            sm_inp = inlist->goto_next();
        }
        delete inlist;
        if (!success) {
            dobjerr("The module %s lacks any %s input.", modname,
                                                     input_name.c_str());
            invalidate();
            return false;
        }
        if (jwm.is_verbose()) {
            std::cout << "\n    " << input_name;
            std::cout << " <-- " << out_modname;
            std::cout << " " << output_name;
        }
        strm >> input_name;
    }while (!strm.eof());
    return true;
}

bool inputedit::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
        case param::STR_UNNAMED:
            if (!set_modname((const char*)data)) {
                dobjerr("Input module %s does not exist.",
                                            (const char*)data);
                invalidate();
                return false;
            }
            return true;
        case param::STR_LIST:
            set_iostr((const char*)data);
            return true;
        default:
            return false;
    }
}

const void* inputedit::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::STR_UNNAMED:   return get_modname();
        case param::STR_LIST:      return get_iostr();
        default: return 0;
    }
}

void inputedit::init_first()
{
    if (done_first())
        return;
    register_param(param::STR_UNNAMED);
    register_param(param::STR_LIST);
}



