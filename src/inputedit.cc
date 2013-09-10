#ifndef INPUTEDIT_H
#include "../include/inputedit.h"
#include "../include/connectorlist.h"

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
    synthmod* sm =
                synthmod::get_modlist()->get_synthmod_by_name(n);
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

bool inputedit::create_connectors(bool verbose)
{
    synthmod* in_sm;
    synthmodlist* smlist = synthmod::get_modlist();
    if (!(in_sm = smlist->get_synthmod_by_name(modname))) {
        *err_msg = "input module ";
        *err_msg += modname;
        *err_msg += " does not exist.";
        invalidate();
        return false;
    }
    inputnames* innames = synthmod::get_inputnames();
    stringstream strm;
    strm << iostr;
    string input_name;
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
        string out_modname;
        string output_name;
        outputnames* outnames = synthmod::get_outputnames();
        outputnames::OUT_TYPE out_type;
        strm >> out_modname;
        if (strcmp(out_modname.c_str(), "off") == 0) {
            IOCAT iocat = innames->get_category(in_type);
            out_type = outnames->get_nonezerotype(iocat);
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
        modinputlist* inlist =
            synthmod::get_inputlist()->get_inputlist_for_module(in_sm);
        modinput* sm_inp = inlist->goto_first();
        bool success = false;
        while(sm_inp && !success) {
            if (sm_inp->get_inputtype() == in_type) {
                connector* con;
                connectorlist* conlist = synthmod::get_connectlist();
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
        if (verbose) {
            cout << "\n    " << input_name;
            cout << " <-- " << out_modname;
            cout << " " << output_name;
        }
        strm >> input_name;
    }while (!strm.eof());
    return true;
}

bool inputedit::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_STR_UNNAMED:
        if (!set_modname((char*)data)) {
            *err_msg = "input module ";
            *err_msg += (char*)data;
            *err_msg += " does not exist.";
            invalidate();
            retv = false;
        }
        else retv = true;
        break;
    case paramnames::PAR_STR_LIST:
        set_iostr((char*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* inputedit::get_param(paramnames::PAR_TYPE dt)
{
    const void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_STR_UNNAMED:
        retv = get_modname();
        break;
    case paramnames::PAR_STR_LIST:
        retv = get_iostr();
        break;
    default:
        retv = 0;
    }
    return retv;
}

void inputedit::create_params()
{
    if (done_params == true)
        return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_EDIT_INPUT, paramnames::PAR_STR_UNNAMED);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_EDIT_INPUT, paramnames::PAR_STR_LIST);
    done_params = true;
}

bool inputedit::done_params = false;

#endif
