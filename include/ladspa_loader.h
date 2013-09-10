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

class ladspa_lib
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
    linkedlist*  plugins;
    ladspa_plug* plug;
    ll_item*     plug_item;

    ladspa_plug* add_plug(ladspa_plug* lp) {
        return plug = (ladspa_plug*)
            (plug_item = plugins->add_at_tail(lp))->get_data();
    }
    ladspa_plug* goto_first() {
        return plug = (ladspa_plug*)
            (plug_item = plugins->goto_first())->get_data();
    }
    ladspa_plug* goto_last() {
        return plug = (ladspa_plug*)
            (plug_item = plugins->goto_last())->get_data();
    }
    ladspa_plug* goto_prev() {
        return plug = (ladspa_plug*)
            (plug_item = plugins->goto_prev())->get_data();
    }
    ladspa_plug* goto_next() {
        return plug = (ladspa_plug*)
            (plug_item = plugins->goto_next())->get_data();
    }

};

//-------------------------------------------------------------

class ladspa_loader
{
 public:
    ladspa_loader();
    ~ladspa_loader();

    ladspa_plug* get_plugin(const char* filename, const char* label);
    std::string get_error_msg(){ return err_msg; }

 private:
    linkedlist* ladspa_libs;
    ladspa_lib* lib;
    ll_item*    lib_item;
    std::string      err_msg;

    void* dlopen_plugin(const char* filename, int flag);

    ladspa_lib* add_lib(ladspa_lib* ll) {
        return lib = (ladspa_lib*)
            (lib_item = ladspa_libs->add_at_tail(ll))->get_data();
    }
    ladspa_lib* goto_first() {
        return lib = (ladspa_lib*)
            (lib_item = ladspa_libs->goto_first())->get_data();
    }
    ladspa_lib* goto_last() {
        return lib = (ladspa_lib*)
            (lib_item = ladspa_libs->goto_last())->get_data();
    }
    ladspa_lib* goto_prev() {
        return lib = (ladspa_lib*)
            (lib_item = ladspa_libs->goto_prev())->get_data();
    }
    ladspa_lib* goto_next() {
        return lib = (ladspa_lib*)
            (lib_item = ladspa_libs->goto_next())->get_data();
    }
};

// partially based upon load.c by Richard W.E. Furse, as provided within
// the LADSPA SDK

#endif
#endif
