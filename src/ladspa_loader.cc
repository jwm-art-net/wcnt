#ifdef WITH_LADSPA

#include "../include/textstuff.h"
#include "../include/ladspa_loader.h"

// rehashed code from the ladspa sdk load.c

#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <limits.h>

#include "../include/globals.h"


#ifdef DEBUG
#define ladspa_err(fmt, ... )                           \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(ladspa_err_msg, STRBUFLEN, fmt, __VA_ARGS__);  \
}
#else
#define ladspa_err(fmt, ... ) \
    cfmt(ladspa_err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif




//-------------------------------------------------------------

static char ladspa_err_msg[STRBUFLEN] = "";


ladspa_plug::ladspa_plug(const LADSPA_Descriptor* descr) :
 label(0), descriptor(descr)
{
    label = descriptor->Label;
}

ladspa_plug::~ladspa_plug()
{
}

LADSPA_Handle ladspa_plug::instantiate()
{
    if (descriptor == 0 || label == 0){
        ladspa_err("%s", "Could not instantiate LADSPA plugin... "
                           "Fault in all likelyhood lies with wcnt.");
        return 0;
    }
    if (descriptor->instantiate == 0){
        ladspa_err("LADSPA plugin %s lacks an instantiate function... "
                           "Maybe it's not a LADSPA plugin?", label);
        return 0;
    }
    LADSPA_Handle lh;
    if(!(lh = descriptor->instantiate(descriptor, wcnt::jwm.samplerate())))
    {
        ladspa_err("Failed to instantiate LADSPA plugin %s.", label);
        return 0;
    }
    return lh;
}


int ladspa_plug::get_port_index(const char* port)
{
    unsigned long portix;

    if (descriptor->PortCount > INT_MAX)
        return -1; // error: plugin author is mental.

    for (portix = 0; portix < descriptor->PortCount; ++portix) {
        if (strcmp(port, descriptor->PortNames[portix]) == 0)
            return portix;
    }

    return -1;
}


int ladspa_plug::connect_port(LADSPA_Handle handle, const char* port, LADSPA_Data* data)
{
    int portix = get_port_index(port);

    debug("Connecting port '%s'...\n", port);

    if (portix == -1) {
        debug("Invalid port '%s'\n", port);
        return -1;
    }

    descriptor->connect_port(handle, portix, data);
    return portix;
}


char* ladspa_plug::validate_port(const char* port, LADSPA_Data* data)
{
    int portix = get_port_index(port);
    const int SZ = 255;
    char buf[SZ] = "";

    if (portix == -1) {
        snprintf(buf, SZ, "Invalid port name '%s'.", port);
        return strdup(buf);
    }

    debug("Validating port: '%s' value: %f\n", port, *data);

    if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[portix])) {
        if (LADSPA_IS_PORT_CONTROL(descriptor->PortDescriptors[portix])) {
            LADSPA_PortRangeHintDescriptor hint =
                        descriptor->PortRangeHints[portix].HintDescriptor;
            if (LADSPA_IS_HINT_BOUNDED_BELOW(hint)) {
                LADSPA_Data bound =
                        descriptor->PortRangeHints[portix].LowerBound;
                if (LADSPA_IS_HINT_SAMPLE_RATE(hint))
                    bound *= wcnt::jwm.samplerate();
                if (bound != 0 && *data < bound) {
                    snprintf(buf, SZ, "should be above %f", bound);
                    return strdup(buf);
                }
            }

            if (LADSPA_IS_HINT_BOUNDED_ABOVE(hint)) {
                LADSPA_Data bound =
                        descriptor->PortRangeHints[portix].UpperBound;
                if (LADSPA_IS_HINT_SAMPLE_RATE(hint) && bound != 0)
                    bound *= wcnt::jwm.samplerate();
                if (bound != 0 && *data > bound) {
                    snprintf(buf, SZ, "should be below %f", bound);
                    return strdup(buf);
                }
            }

            return 0;
        }
    }

    return strdup("Invalid attempt to validate!");
}


bool ladspa_plug::get_port_lower_bound(int portix, LADSPA_Data* result)
{
    LADSPA_PortRangeHintDescriptor hint =
                descriptor->PortRangeHints[portix].HintDescriptor;

    if (LADSPA_IS_HINT_BOUNDED_BELOW(hint)) {
        *result = descriptor->PortRangeHints[portix].LowerBound;
        if (LADSPA_IS_HINT_SAMPLE_RATE(hint))
            *result *= wcnt::jwm.samplerate();
        return true;
    }

    *result = 0;
    return false;
}


bool ladspa_plug::get_port_upper_bound(int portix, LADSPA_Data* result)
{
    LADSPA_PortRangeHintDescriptor hint =
                descriptor->PortRangeHints[portix].HintDescriptor;

    if (LADSPA_IS_HINT_BOUNDED_ABOVE(hint)) {
        *result = descriptor->PortRangeHints[portix].UpperBound;
        if (LADSPA_IS_HINT_SAMPLE_RATE(hint))
            *result *= wcnt::jwm.samplerate();
        return true;
    }

    *result = 0;
    return false;
}

//-------------------------------------------------------------

ladspa_lib::ladspa_lib(const char* fname, LADSPA_Handle handle) :
 filename(fname), lib_handle(handle)
{
}

ladspa_lib::~ladspa_lib()
{
    dlclose(lib_handle);
}

ladspa_plug* ladspa_lib::get_plugin(const char* name)
{
    if(name == 0)
        return 0;
    ladspa_plug* plug = goto_first();
    while(plug){
        if (strcmp(name, plug->get_label()) == 0)
            return plug;
        plug = goto_next();
    }
    const LADSPA_Descriptor* ldescr;
    //ladspa_func_grab lfg;
    LADSPA_Descriptor_Function descrfunc;
    unsigned long plug_ix;
    dlerror();
    descrfunc = (LADSPA_Descriptor_Function) dlsym(lib_handle, "ladspa_descriptor");
    char* dlerr = dlerror();
    if(dlerr) {
        debug("failed to get descriptor function. reason: %s\n", dlerr);
        return 0;
    }

    for(plug_ix = 0;; plug_ix++) {
        if((ldescr = (descrfunc)(plug_ix)) == 0) {
            debug("Plugin count for %s: %lu\n", filename, plug_ix);
            return 0;
        }
        debug("checking descriptor '%s' for match with '%s'\n", ldescr->Label, name);
        if(strcmp(ldescr->Label, name) == 0)
            break;
    }
    plug = new ladspa_plug(ldescr);
    if(add_at_tail(plug) == 0){
        if(plug) delete plug;
        return 0;
    }
    return plug;
}

//-------------------------------------------------------------

ladspa_loader::ladspa_loader()
{
}

ladspa_loader::~ladspa_loader()
{
}

ladspa_plug*
ladspa_loader::get_plugin(const char* fname, const char* label)
{
    if(!fname || !label){
        ladspa_err("Loading of LADSPA plugin %s halted.",
                    (fname ? fname : (label ? label : "")));
        return 0;
    }
    // see if plugin lib is already loaded...
    ladspa_lib* lib = goto_first();
    while(lib){
        if (strcmp(lib->get_filename(), fname) == 0)
            return lib->get_plugin(label);
        lib = goto_next();
    }
    // requested plugin lib not yet loaded...
    LADSPA_Handle lhandle;
    if (!(lhandle = dlopen_plugin(fname, RTLD_NOW))){
        debug("dlopen_plugin '%s' '%s' failed\n", fname, label);
        ladspa_err("Could not open LADSPA plugin %s %s. Please ensure "
                    "the LADSPA_PATH environment variable is set.",
                                                        fname, label);
        return 0;
    }
    if (!(lib = new ladspa_lib(fname, lhandle))) {
        debug("failed to get new ladspa_lib for plugin '%s' '%s'\n", fname, label);
    }
    if (add_at_tail(lib) == 0){
        if (lib)
            delete lib;
        ladspa_err("Library for LADSPA plugin %s %s loaded ok, but "
                   "something else has gone wrong!", fname, label);
        return 0;
    }
    return lib->get_plugin(label);
}

void* ladspa_loader::dlopen_plugin(const char* fname, int flag)
{
    char* buf;
    const char * start;
    const char * end;
    int end_in_so;
    int fnlen = strlen(fname);
    void* result = 0;
    if (fname[0] == '/') {
        result = dlopen(fname, flag);
        if (result != NULL)
        return result;
    }
    else {
        if(!(start = getenv("LADSPA_PATH")))
            start = wcnt::ladspa_path_if_env_not_set;
        while (*start != '\0') {
            end = start;
            while (*end != ':' && *end != '\0') end++;
            buf = new char[fnlen + 2 + (end - start)];
            if (end > start)
                strncpy(buf, start, end - start);
            int need_slash = 0;
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


const char* ladspa_loader::get_error_msg()
{
    return ladspa_err_msg;
}


#endif
