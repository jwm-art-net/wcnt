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
    void activate_custom_ui_items() { custom = true; }
    void deactivate_custom_ui_items() { custom = false; }

 private:
    char* filename;
    char* label;
    ui::moditem_list*        plugin_items;
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_handle;
    ladspa_plug*             lp;
    int port_count;

    typedef struct _port_mapping
    {
        int id;         // id registered via register_(param|input|output)
        int type;       // ui::TYPE (UI_PARAM|UI_INPUT|UI_OUTPUT)
        LADSPA_Data     control;    // used if type is param
        LADSPA_Data*    buf;        // used if type is input|output
        double          param;
        double*         input;
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
