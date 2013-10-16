#ifndef MODPART_H
#define MODPART_H

#include "inputnames.h"
#include "paramnames.h"
#include "outputnames.h"
#include "stockerrs.h"
#include "types.h"


namespace synthmod { class base; }


namespace modpart
{
 class base
 {
  public:
    base(synthmod::base* inheritor);

    virtual ~base(){};

    /* virtuals */
    virtual void run() = 0;
    virtual void init() {};

    /* input/output/param access */
    virtual const void* set_in(input::TYPE, const void*);
    virtual const void* get_in(input::TYPE) const;
    virtual bool        set_param(param::TYPE, const void*);
    virtual const void* get_param(param::TYPE) const;
    virtual const void* get_out(output::TYPE) const;

    /* duplicate module can't be const due to my linked_list impl. */
    virtual base* duplicate_modpart();

    // validation
    virtual errors::TYPE validate()
        { return errors::NO_ERROR; }

    #ifdef DEBUG
    bool check_inputs();
    #endif

  protected:
    bool sm_done_first();
//    void invalidate()           { flags &=~ synthmod::SM_VALID; }

    /*  first instance initializations (ie parameter and data object
        registration) should be performed in the register_ui method of
        derived objects.
     */

    void register_sm_param(param::TYPE);
    void register_sm_param(param::TYPE, const char* fixed_string);

    /*  inputs & outputs OTOH, are unique to each instance, so will need
        registration per instance (ie in derived constructor).
    */
    void register_sm_input(input::TYPE);

    bool validate_sm_param(param::TYPE, errors::TYPE);

    static char err_msg[STRBUFLEN];

  private:
    synthmod::base* sm;
    int flags;
//  not to be used:
    base();
    base(const base &);
 };
}; // namespace modpart

#endif
