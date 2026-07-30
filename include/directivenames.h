#ifndef DIRECTIVENAMES_H
#define DIRECTIVENAMES_H

#include "getnames.h"
#include "iocat.h"

namespace directive
{
    enum TYPE
    {
        ERR_TYPE,
        AUTOGROUP_CREATE,
        AUTOGROUP_ADD,
        // AUTOGROUP_STOP, unnecessary
        SAMPLE_RATE,
        LAST_TYPE
    };

    class names : public getnames<TYPE, iocat::TYPE>
    {
      public:
        static void instantiate() { static names directivenames; }


      private:
        names() : getnames(LAST_TYPE, data) {}
        ~names() {}
        static const struct gn_data data[LAST_TYPE];
    };

}; // namespace directive

#endif
