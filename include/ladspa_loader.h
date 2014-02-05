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
