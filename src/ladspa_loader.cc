#ifndef LADSPA_LOAD_H
#ifdef WITH_LADSPA

#include "../include/ladspa_loader.h"

// rehashed code from the ladspa sdk load.c

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/jwm_globals.h"

//-------------------------------------------------------------

ladspa_plug::ladspa_plug(const LADSPA_Descriptor* descr) :
 label(0), descriptor(descr)
{
    label = descriptor->Label;
}

ladspa_plug::~ladspa_plug()
{
}

const char* ladspa_plug::get_label()
{
    if (label == 0) return 0;
    return label;
}

const LADSPA_Descriptor* ladspa_plug::get_descriptor()
{
    if(descriptor == 0) return 0;
    return descriptor;
}


LADSPA_Handle ladspa_plug::instantiate()
{
    if (descriptor == 0 || label == 0){
        err_msg = "Could not instantiate LADSPA plugin..."
                  "Fault in all likelyhood lies with wcnt.";
        return 0;
    }
    if (descriptor->instantiate == 0){
        err_msg = "LADSPA plugin "; err_msg += label;
        err_msg += " lacks an instantiate function... Maybe "
                   "it's not a LADSPA plugin?";
        return 0;
    }
    LADSPA_Handle lh;
    if(!(lh = descriptor->instantiate(descriptor, jwm.samplerate())))
    {
        err_msg = "Failed to instantiate LADSPA plugin ";
        err_msg += label;
        return 0;
    }
    return lh;
}

//-------------------------------------------------------------

ladspa_lib::ladspa_lib(const char* fname, LADSPA_Handle handle) :
 filename(fname), lib_handle(handle),
 plugins(0), plug(0), plug_item(0)
{
    plugins = new linkedlist(linkedlist::MULTIREF_OFF,
                             linkedlist::NO_NULLDATA);
}

ladspa_lib::~ladspa_lib()
{
    if (plugins){
        goto_first();
        while(plug){
            delete plug;
            goto_next();
        }
        delete plugins;
    }
    dlclose(lib_handle);
}

ladspa_plug* ladspa_lib::get_plugin(const char* name)
{
    if(plugins == 0 || name == 0)
        return 0;
    goto_first();
    while(plug){
        if (strcmp(name, plug->get_label()) == 0)
            return plug;
        goto_next();
    }
    const LADSPA_Descriptor* ldescr;
    ladspa_func_grab lfg;
    LADSPA_Descriptor_Function descrfunc;
    unsigned long plug_ix;
    dlerror();
    lfg.dlsym_ret = dlsym(lib_handle, "ladspa_descriptor");
    descrfunc = lfg.descrfunc;
    if(!descrfunc)
        return 0;
    for(plug_ix = 0;; plug_ix++) {
        if((ldescr = (descrfunc)(plug_ix)) == 0)
            return 0;
        if(strcmp(ldescr->Label, name) == 0)
            break;
    }
    ladspa_plug* lp = new ladspa_plug(ldescr);
    if(add_plug(lp) == 0){
        if(lp) delete lp;
        return 0;
    }
    return lp;
}

//-------------------------------------------------------------

ladspa_loader::ladspa_loader() :
 ladspa_libs(0), lib(0), lib_item(0)
{
    ladspa_libs = new linkedlist(linkedlist::MULTIREF_OFF,
                                 linkedlist::NO_NULLDATA);
}

ladspa_loader::~ladspa_loader()
{
    if(ladspa_libs){
        goto_first();
        while(lib){
            delete lib;
            goto_next();
        }
        delete ladspa_libs;
    }
}

ladspa_plug*
ladspa_loader::get_plugin(const char* fname, const char* label)
{
    if(!ladspa_libs || !fname || !label){
        err_msg = "loading of LADSPA plugin ";
        err_msg += fname; err_msg += " ";
        err_msg += label; err_msg += " halted - before it even began...";
        return 0;
    }
    // see if plugin lib is already loaded...
    goto_first();
    while(lib){
        if (strcmp(lib->get_filename(), fname) == 0)
            return lib->get_plugin(label);
        goto_next();
    }
    // requested plugin lib not yet loaded...
    LADSPA_Handle lhandle;
    if (!(lhandle = dlopen_plugin(fname, RTLD_NOW))){
        err_msg = "Could not open LADSPA plugin ";
        err_msg += fname; err_msg += " ";
        err_msg += label;
        err_msg += " - probably could not find library."
                   " Is the LADSPA_PATH environment variable set???";
        return 0;
    }
    ladspa_lib* ll = new ladspa_lib(fname, lhandle);
    if (add_lib(ll) == 0){
        if (ll) delete ll;
        err_msg = "Library for LADSPA plugin ";
        err_msg += fname; err_msg += " ";
        err_msg += label;
        err_msg += " loaded ok, but now something else has gone wrong!";
        return 0;
    }
    return ll->get_plugin(label);
}

void* ladspa_loader::dlopen_plugin(const char* fname, int flag)
{
    char* buf;
    const char * start;
    const char * end;
    const char * ladspa_path;
    int end_in_so;
    int need_slash;
    int fnlen = strlen(fname);
    void* result = 0;
    if (fname[0] == '/') {
        result = dlopen(fname, flag);
        if (result != NULL)
        return result;
    }
    else {
        if((ladspa_path = getenv("LADSPA_PATH"))){
            start = ladspa_path;
            while (*start != '\0') {
                end = start;
                while (*end != ':' && *end != '\0') end++;
                buf = new char[fnlen + 2 + (end - start)];
                if (end > start)
                    strncpy(buf, start, end - start);
                need_slash = 0;
                if (end > start)
                    if (*(end - 1) != '/') {
                        need_slash = 1;
                        buf[end - start] = '/';
                    }
                strcpy(buf + need_slash + (end - start), fname);
                result = dlopen(buf, flag);
                delete [] buf;
                if(result != 0)
                    return result;
                start = end;
                if (*start == ':')
                    start++;
            }
        }
    }
    end_in_so = 0;
    if (fnlen > 3)
        end_in_so = (strcmp(fname + fnlen - 3, ".so") == 0);
    if (!end_in_so) {
        buf = new char[fnlen + 4];
        strcpy(buf, fname);
        strcat(buf, ".so");
        result = dlopen_plugin(buf, flag);
        delete [] buf;
    }
    if (result != NULL)
        return result;
    return dlopen(fname, flag);
}

#endif
#endif
