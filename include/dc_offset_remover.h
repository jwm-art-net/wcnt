#ifndef DC_OFFSET_REMOVER_H
#define DC_OFFSET_REMOVER_H
#ifdef WITH_LADSPA

#include "synthmod.h"
#include "ladspa_loader.h"

/*

Plugin Name: "DC Offset Remover"
Plugin Label: "dcRemove"
Plugin Unique ID: 1207
Maker: "Steve Harris <steve@plugin.org.uk>"
Copyright: "GPL"
Must Run Real-Time: No
Has activate() Function: Yes
Has deactivate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports:  "Input" input, audio
        "Output" output, audio

*/

class dc_offset_remover : public synthmod
{
public:
    dc_offset_remover(const char*);
    ~dc_offset_remover();
    void run();
    void init();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;

private:
    // inputs
    const double* input;
    // output
    double output;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data* l_input;
    LADSPA_Data* l_output;
};

#endif /* WITH_LADSPA */
#endif /* DC_OFFSET_REMOVER */
