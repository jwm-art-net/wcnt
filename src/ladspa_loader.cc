#ifdef WITH_LADSPA

#include "../include/textstuff.h"
#include "../include/ladspa_loader.h"

// rehashed code from the ladspa sdk load.c

#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>

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
    unsigned long px;

    if (descriptor->PortCount > INT_MAX)
        return -1; // error: plugin author is mental.

    for (px = 0; px < descriptor->PortCount; ++px) {
        if (strcasecmp(port, descriptor->PortNames[px]) == 0)
            return px;
    }

    return -1;
}


int ladspa_plug::connect_port(LADSPA_Handle handle, const char* port, LADSPA_Data* data)
{
    int px = get_port_index(port);

    debug("Connecting port '%s'...\n", port);

    if (px == -1) {
        debug("Invalid port '%s'\n", port);
        return -1;
    }

    descriptor->connect_port(handle, px, data);
    return px;
}


char* ladspa_plug::validate_port(const char* port, LADSPA_Data* data)
{
    int px = get_port_index(port);
    if (px == -1) {
        const int SZ = 255;
        char buf[SZ] = "";
        snprintf(buf, SZ, "Invalid port name '%s'.", port);
        return new_strdup(buf);
    }
    return validate_port(get_port_index(port), data);
}


char* ladspa_plug::validate_port(int px, LADSPA_Data* data)
{
    const int SZ = 255;
    char buf[SZ] = "";

    if (px < 0 || px > (int)descriptor->PortCount) {
        snprintf(buf, SZ, "Invalid port id '%d'.", px);
        return new_strdup(buf);
    }

    debug("Validating port: '%s' value: %f\n", descriptor->PortNames[px], *data);

    if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[px])) {
        if (LADSPA_IS_PORT_CONTROL(descriptor->PortDescriptors[px])) {
            LADSPA_PortRangeHintDescriptor hint =
                        descriptor->PortRangeHints[px].HintDescriptor;
            if (LADSPA_IS_HINT_BOUNDED_BELOW(hint)) {
                LADSPA_Data bound =
                        descriptor->PortRangeHints[px].LowerBound;
                if (LADSPA_IS_HINT_SAMPLE_RATE(hint))
                    bound *= wcnt::jwm.samplerate();
                if (bound != 0 && *data < bound) {
                    snprintf(buf, SZ, "(%f) should be above %f", *data, bound);
                    return new_strdup(buf);
                }
            }

            if (LADSPA_IS_HINT_BOUNDED_ABOVE(hint)) {
                LADSPA_Data bound =
                        descriptor->PortRangeHints[px].UpperBound;
                if (LADSPA_IS_HINT_SAMPLE_RATE(hint) && bound != 0)
                    bound *= wcnt::jwm.samplerate();
                if (bound != 0 && *data > bound) {
                    snprintf(buf, SZ, "should be below %f", bound);
                    return new_strdup(buf);
                }
            }

            return 0;
        }
    }

    return new_strdup("Invalid attempt to validate!");
}


bool ladspa_plug::get_port_lower_bound(int px, LADSPA_Data* result)
{
    LADSPA_PortRangeHintDescriptor hint =
                descriptor->PortRangeHints[px].HintDescriptor;

    if (LADSPA_IS_HINT_BOUNDED_BELOW(hint)) {
        *result = descriptor->PortRangeHints[px].LowerBound;
        if (LADSPA_IS_HINT_SAMPLE_RATE(hint))
            *result *= wcnt::jwm.samplerate();
        return true;
    }

    *result = 0;
    return false;
}


bool ladspa_plug::get_port_upper_bound(int px, LADSPA_Data* result)
{
    LADSPA_PortRangeHintDescriptor hint =
                descriptor->PortRangeHints[px].HintDescriptor;

    if (LADSPA_IS_HINT_BOUNDED_ABOVE(hint)) {
        *result = descriptor->PortRangeHints[px].UpperBound;
        if (LADSPA_IS_HINT_SAMPLE_RATE(hint))
            *result *= wcnt::jwm.samplerate();
        return true;
    }

    *result = 0;
    return false;
}

//-------------------------------------------------------------

ladspa_lib::ladspa_lib( const char* _path,
                        LADSPA_Handle _handle,
                        LADSPA_Descriptor_Function _descrfunc) :
 path(), handle(_handle), descrfunc(_descrfunc)
{
    path = new_strdup(_path);
}


ladspa_lib::~ladspa_lib()
{
    if (handle)
        dlclose(handle);
    if (path)
        delete [] path;
}

int ladspa_lib::filename_cmp(const ladspa_lib* lib2) const
{
    const char* fn1 = strrchr(path, '/');
    const char* fn2 = strrchr(lib2->get_path(), '/');

    if (!fn1 && !fn2)
        return 0;

    if (!fn1)
        return -1;
    if (!fn2)
        return 1;

    fn1++;
    fn2++;

    return strcmp(fn1, fn2);
}


int ladspa_lib::get_plugin_count()
{
    const LADSPA_Descriptor* descr = 0;

    for (int px = 0;; ++px) {
        if (!(descr = (descrfunc)(px)))
            return px;
    }
    debug("LADSPA plugin count (logic?) error!\n");
    return -1;
}



ladspa_plug* ladspa_lib::get_plugin(const char* name)
{
    if(name == 0)
        return 0;

    ladspa_plug* plug = goto_first();

    while(plug){
        if (strcasecmp(name, plug->get_label()) == 0)
            return plug;
        plug = goto_next();
    }

    const LADSPA_Descriptor* descr = 0;

    for (int plug_ix = 0;; plug_ix++) {
        if (!(descr = (descrfunc)(plug_ix))) {
            debug("Plugin count for %s: %d\n", path, plug_ix);
            return 0;
        }
        debug("checking descriptor '%s' for match with '%s'\n", descr->Label,
                                                                       name);
        if(strcasecmp(descr->Label, name) == 0)
            break;
    }

    plug = new ladspa_plug(descr);

    if (add_at_tail(plug) == 0){
        if (plug)
            delete plug;
        return 0;
    }
    return plug;
}


void ladspa_lib::load_all()
{

}


//-------------------------------------------------------------

ladspa_loader::ladspa_loader()
{
}

ladspa_loader::~ladspa_loader()
{
}

char* ladspa_loader::find_lib_path(const char* name)
{
    if (!name) {
        debug("NULL filename.\n");
        return 0;
    }

    size_t len = strlen(name);
    bool is_so = false;
    if (len > 3)
        is_so = (strcasecmp(name + len - 3, ".so") == 0);

    if (name[0] == '/') {
        if (is_so)
            return new_strdup(name);
        char* buf = new char[len + 3 + 1];
        strcpy(buf, name);
        strcpy(buf + len, ".so");
        return buf;
    }

    const char* start = 0;
    const char* end = 0;

    if (!(start = getenv("LADSPA_PATH")))
        start = wcnt::ladspa_path_if_env_not_set;

    while (*start != '\0') {
        end = start;
        for (end = start; *end != ':' && *end != '\0'; ++end);
        char* buf = new char[len + 1 + (end - start) + (is_so ? 0 : 3)];
        if (end > start)
            strncpy(buf, start, end - start);

        int need_slash = 0;
        if (end > start) {
            if (*(end - 1) != '/') {
                need_slash = 1;
                buf[end - start] = '/';
            }
        }
        char* p = buf + need_slash + (end - start);
        strcpy(p, name);
        if (!is_so)
            strcpy(p + len, ".so");

        struct  stat statbuf;
        int     status;
        if (!(status = stat(buf, &statbuf))) {
            debug("found:'%s'\n", buf);
            return buf;
        }
        delete [] buf;
        start = end;
        if (*start == ':')
            start++;
    }

    return 0;
}


ladspa_lib* ladspa_loader::load_lib(const char* path)
{
    if (!path) {
        debug("***** Unspecified path *****\n");
        return 0;
    }

    if (path[0] != '/') {
        debug("***** Relative path *****\n");
        return 0;
    }

    ladspa_lib* lib = goto_first();
    while (lib) {
        if (strcasecmp(lib->get_path(), path) == 0)
            return lib;
    }

    void* handle = dlopen(path, RTLD_LAZY);

    if (!handle) {
        debug("Failed to open library '%s'\n", path);
        return 0;
    }

    dlerror();
    LADSPA_Descriptor_Function descrfunc =
                    (LADSPA_Descriptor_Function)
                                    dlsym(handle, "ladspa_descriptor");
    char* dlerr = dlerror();
    if (dlerr) {
        debug("LADSPA Descriptor function not found.\n");
        dlclose(handle);
        free(dlerr);
        return 0;
    }

    if (!(lib = new ladspa_lib(path, handle, descrfunc))) {
        debug("Failed to allocate ladspa_lib data\n");
        dlclose(handle);
        return 0;
    }

    if (!add_at_tail(lib)) {
        dlclose(handle);
        delete lib;
        debug("Failed to add ladspa_lib data\n");
        return 0;
    }

    return lib;
}

ladspa_plug*
ladspa_loader::get_plugin(const char* path, const char* label)
{
    if (!path || !label){
        ladspa_err("Loading of LADSPA plugin %s halted.",
                    (path ? path : (label ? label : "")));
        return 0;
    }
    // see if plugin lib is already loaded...
    ladspa_lib* lib = goto_first();
    while(lib){
        if (strcasecmp(lib->get_path(), path) == 0)
            return lib->get_plugin(label);
        lib = goto_next();
    }

    if (!(lib = load_lib(path))) {
        ladspa_err("Failed to load LADSPA library '%s'\n", path);
        return 0;
    }

    return lib->get_plugin(label);
}


void ladspa_loader::load_all()
{
    const char* start = 0;
    const char* end = 0;

    if (!(start = getenv("LADSPA_PATH")))
        start = wcnt::ladspa_path_if_env_not_set;

    while (*start != '\0') {
        end = start;
        for (end = start; *end != ':' && *end != '\0'; ++end);
        char* path = new char[(end - start) + 2];
        strncpy(path, start, end - start);

        int need_slash = 0;
        if (*(end - 1) != '/') {
            need_slash = 1;
            path[end - start] = '/';
        }

        path[(end - start) + need_slash] = '\0';

        DIR* dir = opendir(path);
        if (dir) {
            size_t plen = strlen(path);
            struct dirent* de = 0;
            debug("Reading dir '%s'...\n", path);
            int count = 0;
            do {
                de = readdir(dir);
                if (de) {
                    char* libfname = new char[plen + strlen(de->d_name) + 1];
                    strcpy(libfname, path);
                    strcpy(libfname + plen, de->d_name);
                    if (quick_open_lib(libfname))
                        ++count;
                    delete [] libfname;
                }
            } while(de);
            closedir(dir);
            debug("%d LADSPA libraries found.\n", count);
        }
        else {
            debug("Failed opendir '%s'\n", path);
        }

        delete [] path;
        start = end;
        if (*start == ':')
            start++;
    }
}

ladspa_lib* ladspa_loader::quick_open_lib(const char* path)
{
    ladspa_lib* lib = 0;

    void* handle = dlopen(path, RTLD_LAZY);

    if (!handle)
        return 0;

    dlerror();
    LADSPA_Descriptor_Function descrfunc =
                    (LADSPA_Descriptor_Function)
                                    dlsym(handle, "ladspa_descriptor");
    char* dlerr = dlerror();

    if (dlerr) {
        debug("not a LADSPA library '%s'\n", path);
        return 0;
    }

    if (!(lib = new ladspa_lib(path, handle, descrfunc))) {
        debug("Failed to allocate ladspa_lib data\n");
        dlclose(handle);
        return 0;
    }

    if (!add_at_tail(lib)) {
        dlclose(handle);
        delete lib;
        debug("Failed to add ladspa_lib data\n");
        return 0;
    }

    return lib;
}


/*
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
        end_in_so = (strcasecmp(fname + fnlen - 3, ".so") == 0);
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

*/

const char* ladspa_loader::get_error_msg()
{
    return ladspa_err_msg;
}


#endif
