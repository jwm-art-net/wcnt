#ifndef LADSPA_LOADER_H
#ifdef WITH_LADSPA
#define LADSPA_LOADER_H

#include "ladspa.h"
#include "linkedlist.h"

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

    // PortCount is defined in ladspa.h as unsigned long.
    // here however, we refuse to accomodate plugins with more than INT_MAX
    // ports...

    int get_port_index(const char* port);

    // connect_port returns the index number of the found port, -1 otherwise.
    int connect_port(LADSPA_Handle, const char* port, LADSPA_Data*);

    // validate_port returns an error string on invalid port data:
    // the string should be free'd.
    char* validate_port(const char* port, LADSPA_Data* data);

    bool get_port_lower_bound(int port_index, LADSPA_Data* result);
    bool get_port_upper_bound(int port_index, LADSPA_Data* result);

 private:
    const char* label;
    const LADSPA_Descriptor* descriptor;
};

//-------------------------------------------------------------

class ladspa_lib : public linked_list<ladspa_plug>
{
 public:
    ladspa_lib(const char* path, LADSPA_Handle, LADSPA_Descriptor_Function);
    ~ladspa_lib();

    const char*                get_path()       const { return path;  }
    LADSPA_Handle              get_handle()     const { return handle;}
    LADSPA_Descriptor_Function get_descr_func() const { return descrfunc; }

    ladspa_plug*               get_plugin(const char* name);

    void                       load_all();

 private:
    const char*                path;
    LADSPA_Handle              handle;
    LADSPA_Descriptor_Function descrfunc;
};

//-------------------------------------------------------------

class ladspa_loader : public linked_list<ladspa_lib>
{
 public:
    ladspa_loader();
    ~ladspa_loader();

    // returns absolute path of lib specified by filename. filename
    // may be an absolute path. returned value should be delete [] ed.
    // does not guarantee the path found points to a ladspa plugin lib.
    char*        find_lib_path(const char* lib_name);
    ladspa_lib*  load_lib(const char* path);
    ladspa_plug* get_plugin(const char* path, const char* label);

    void         load_all(); // load all LADSPA libraries...

    static const char* get_error_msg();

    private:
    ladspa_lib* quick_open_lib(const char* path);

};

// partially based upon load.c by Richard W.E. Furse, as provided within
// the LADSPA SDK


#endif
#endif
