#include "../include/directivenames.h"

namespace directive
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,             "BAD_DIRECTIVE_TYPE",   iocat::ERR_TYPE,
        "DIRECTIVE_ERROR"   ,0},
    { AUTOGROUP_CREATE,     "autogroup_create",     iocat::STRING,
        "Name of group to create to autogroup modules into." ,0},
    { AUTOGROUP_ADD,        "autogroup_add",        iocat::STRING,
        "Name of existing group to autogroup additional modules into." ,0},
    { AUTOCHAIN_CREATE,     "autochain_create",     iocat::STRING,
        "Name of chain to create to autochain modules into.", 0},
    { AUTOCHAIN_COPY_ONLY,  "autochain_copy_only",  iocat::STATE,
        "When set on, any subsequent modules created are placed directly into the chain's private internal copy, instead of regular user space. Setting this off again restores regular creation of modules during autochaining. The setting applies per-chain.", 0},
    { SAMPLE_RATE,          "samplerate",           iocat::SAMP_T,
        "Sets the sample rate for the entire project to run at, including any WAV files written.", 0}
 };
}; // namespace directive
