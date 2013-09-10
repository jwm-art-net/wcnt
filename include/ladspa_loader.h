#ifndef LADSPA_LOADER_H
#ifdef WITH_LADSPA
#define LADSPA_LOADER_H

#include <string>

#include "ladspa.h"
#include "linkedlist.h"

//-------------------------------------------------------------

union ladspa_func_grab
{
    void* dlsym_ret;
    LADSPA_Descriptor_Function descrfunc;
};

//-------------------------------------------------------------

// forward def:
class ladspa_lib;

// ladspa_plug - details

class ladspa_plug
{
 public:
    ladspa_plug(const LADSPA_Descriptor*);
    ~ladspa_plug();

    const char* get_label();
    const LADSPA_Descriptor* get_descriptor();

    LADSPA_Handle instantiate();

    std::string get_error_msg(){ return err_msg; }

 private:
    const char* label;
    const LADSPA_Descriptor* descriptor;
    std::string err_msg;
};

//-------------------------------------------------------------

class ladspa_lib : public linked_list<ladspa_plug>
{
 public:
    ladspa_lib(const char*, LADSPA_Handle);
    ~ladspa_lib();
    const char* get_filename() { return filename; }
    LADSPA_Handle get_handle() { return lib_handle; }

    ladspa_plug* get_plugin(const char* name);

 private:
    const char*  filename;
    LADSPA_Handle lib_handle;
};

//-------------------------------------------------------------

class ladspa_loader : public linked_list<ladspa_lib>
{
 public:
    ladspa_loader();
    ~ladspa_loader();

    ladspa_plug* get_plugin(const char* filename, const char* label);
    std::string get_error_msg(){ return err_msg; }

 private:
    std::string      err_msg;

    void* dlopen_plugin(const char* filename, int flag);
};

// partially based upon load.c by Richard W.E. Furse, as provided within
// the LADSPA SDK

#endif
#endif