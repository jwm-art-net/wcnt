#ifndef MODPART_H
#define MODPART_H

#include "inputnames.h"
#include "paramnames.h"
#include "outputnames.h"
#include "stockerrs.h"
#include "types.h"
#include "ui_moditem.h"


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
    virtual const void* set_in(int input_type, const void*);
    virtual const void* get_in(int input_type) const;
    virtual bool        set_param(int param_type, const void*);
    virtual const void* get_param(int param_type) const;
    virtual const void* get_out(int output_type) const;

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

    ui::moditem* register_sm_param(int param_type);
    ui::moditem* register_sm_param(int param_type, const char* fixed_string);

    /*  inputs & outputs OTOH, are unique to each instance, so will need
        registration per instance (ie in derived constructor).
    */
    ui::moditem* register_sm_input(int input_type);

    bool validate_sm_param(int param_type, errors::TYPE);

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
