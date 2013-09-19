#ifndef LADSPA_LOADER_H
#ifdef WITH_LADSPA
#define LADSPA_LOADER_H

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

    const char* get_label() const { return label; }
    const LADSPA_Descriptor* get_descriptor() const { return descriptor; }

    LADSPA_Handle instantiate();

 private:
    const char* label;
    const LADSPA_Descriptor* descriptor;
};

//-------------------------------------------------------------

class ladspa_lib : public linked_list<ladspa_plug>
{
 public:
    ladspa_lib(const char*, LADSPA_Handle);
    ~ladspa_lib();
    const char* get_filename() const { return filename; }
    LADSPA_Handle get_handle() const { return lib_handle; }

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

    static const char* get_error_msg();

 private:
    void* dlopen_plugin(const char* filename, int flag);
};

// partially based upon load.c by Richard W.E. Furse, as provided within
// the LADSPA SDK


#endif
#endif
