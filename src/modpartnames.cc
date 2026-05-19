#include "../include/modpartnames.h"

namespace modpart
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE, "BAD_MODPART_TYPE",     CAT_ERR_TYPE, "ERROR!"  ,0 },
    { MP_GAIN,  "gain",     CAT_AMP, "Provides amplitude adjustment and modulation to a module." ,0 }
 };
}; // namespace module

