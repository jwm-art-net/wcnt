#ifndef LADSPA_MODULE_H
#define LADSPA_MODULE_H

#include "synthmod.h"
#include "ladspa_loader.h"


class ladspa_module : public synthmod::base
{
 public:
    ladspa_module(const char*);
    ~ladspa_module();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

    bool create_custom_ui_items();
    bool activate_custom_ui_items();
    void deactivate_custom_ui_items() { custom = false; }

 private:
    char* libname;
    char* label;
    char* path;
    ui::moditem_list*        plugin_items;
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_handle;
    ladspa_plug*             lp;
    int port_count;

    // arrays for inputs and outputs, and for mapping during run()
    int             in_int_count;   // no. of int inputs
    wcint_t**       in_int;         // array of int inputs
    int*            in_int_px;      // port index element refers to

    int             in_float_count;
    double**        in_float;
    int*            in_float_px;

    int             out_int_count;
    wcint_t*        out_int;
    int*            out_int_px;

    int             out_float_count;
    double*         out_float;
    int*            out_float_px;

    int             param_int_count;
    wcint_t*        param_int;
    int*            param_int_px;

    int             param_float_count;
    double*         param_float;
    int*            param_float_px;

    // wcnt only processes a single sample per call to run()...
    LADSPA_Data*    ports;

    typedef struct _port_mapping
    {               // mapping for set/get methods
        int id;     // id registered via register_(param|input|output)
        int type;   // ui::UI_INPUT/OUTPUT/PARAM
        int index;  // index to corresponding i/o/p array.
    } port_map;

    port_map* port_mappings;

    void register_ui();
    bool custom;
    #if DEBUG
    bool custom_created;
    #endif
    ui::moditem_list* get_ui_items();
};

#endif
