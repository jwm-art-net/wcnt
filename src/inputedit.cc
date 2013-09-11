#include "../include/inputedit.h"
#include "../include/connectorlist.h"
#include "../include/synthmodlist.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/dobjparamlist.h"

#include <iostream>
#include <sstream>

inputedit::inputedit() :
 dobj(dobjnames::SIN_EDIT_INPUT),
 modname(0), iostr(0)
{
    create_params();
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
        *err_msg = "input module ";
        *err_msg += modname;
        *err_msg += " does not exist.";
        invalidate();
        return false;
    }
    const inputnames* innames = jwm.get_inputnames();
    std::stringstream strm;
    strm << iostr;
    std::string input_name;
    strm >> input_name;
    do{
        inputnames::IN_TYPE in_type;
        in_type = innames->get_type(input_name.c_str());
        if (in_type == inputnames::IN_FIRST) {
            *err_msg = "\nunrecognised input type ";
            *err_msg += input_name;
            *err_msg += ", in connection for ";
            *err_msg += modname;
            invalidate();
            return false;
        }
        std::string out_modname;
        std::string output_name;
        const outputnames* outnames = jwm.get_outputnames();
        outputnames::OUT_TYPE out_type;
        strm >> out_modname;
        if (strcmp(out_modname.c_str(), "off") == 0) {
            out_type = outnames->get_nonezerotype(
                                innames->get_category(in_type));
        }
        else {
            // allow output module to not exist yet.
            strm >> output_name;
            out_type = outnames->get_type(output_name.c_str());
            if (out_type == outputnames::OUT_FIRST) {
                *err_msg = "unrecognised output type ";
                *err_msg += output_name;
                *err_msg += " in connection for ";
                *err_msg += modname;
                invalidate();
                return false;
            }
            if (innames->get_category(in_type)
                != outnames->get_category(out_type))
            {
                *err_msg = "in connection for ";
                *err_msg += modname;
                *err_msg += " output ";
                *err_msg += output_name;
                *err_msg += " does not match category of input ";
                *err_msg += input_name;
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
            *err_msg = "the module ";
            *err_msg += modname;
            *err_msg += " lacks any ";
            *err_msg += input_name;
            *err_msg += " input";
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

bool inputedit::set_param(paramnames::PAR_TYPE dt, void* data)
{
    switch(dt)
    {
        case paramnames::STR_UNNAMED:
            if (!set_modname((char*)data)) {
                *err_msg = "input module ";
                *err_msg += (char*)data;
                *err_msg += " does not exist.";
                invalidate();
                return false;
            }
            return true;
        case paramnames::STR_LIST:
            set_iostr((char*)data);
            return true;
        default:
            return false;
    }
}

void const* inputedit::get_param(paramnames::PAR_TYPE dt) const
{
    switch(dt)
    {
        case paramnames::STR_UNNAMED:   return get_modname();
        case paramnames::STR_LIST:      return get_iostr();
        default: return 0;
    }
}

void inputedit::create_params()
{
    if (done_params == true)
        return;
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::SIN_EDIT_INPUT, paramnames::STR_UNNAMED);
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::SIN_EDIT_INPUT, paramnames::STR_LIST);
    done_params = true;
}

bool inputedit::done_params = false;

