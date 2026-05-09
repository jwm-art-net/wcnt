#ifndef MODPART_H
#define MODPART_H

#include "inputnames.h"
#include "paramnames.h"
#include "outputnames.h"
#include "stockerrs.h"
#include "types.h"
#include "ui_moditem.h"

/* Really wish that I documented my code more. From what I can tell...
 * modpart = module part
 * - a reusable part which can be common to two or more modules.
 *
 * there is only a single modpart at present gain - see gain.h
 * and it is used by the delay, echo, and monoamp modules.
*/

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
    // register_ui should only be called within synthmod::register_ui.
    virtual void register_ui() = 0;

    ui::moditem* register_sm_param(param::TYPE);
    ui::moditem* register_sm_param(param::TYPE, const char* fixed_string);

    /*  inputs & outputs OTOH, are unique to each instance, so will need
        registration per instance (ie in derived constructor).
    */
    ui::moditem* register_sm_input(input::TYPE);

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
