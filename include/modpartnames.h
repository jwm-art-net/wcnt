#ifndef MODPARTNAMES_H
#define MODPARTNAMES_H

#include "getnames.h"

namespace modpart
{
 enum CAT
 {
    CAT_ERR_TYPE,
    CAT_AMP,
    CAT_FREQ,
    CAT_LAST_TYPE
 };

 enum TYPE
 {
    ERR_TYPE,
    MP_GAIN,
    LAST_TYPE
 };

 class names : public getnames<TYPE, CAT>
 {
  public:
    static void instantiate() { static names modpartnames; }

  private:
    names() : getnames(LAST_TYPE, data) {}
    ~names() {}
    static const struct gn_data data[LAST_TYPE];
 };
};

#endif
