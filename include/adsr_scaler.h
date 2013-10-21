#ifndef ADSR_SCALER_H
#define ADSR_SCALER_H

#include "dobj.h"
#include "adsr.h"

//  adsr_scaler
//  -----------
//
//  this is as close as i'll get to a fully fledged adsr envelope
//  editor.
//
//  what does it do?
//
//  with it you can scale the time length of individual sections of
//  the envelope, that is you can scale the length of the attack or
//  decay or release sections.
//
//  it does not scale individual sub-sections of these sections.
//
//  it might be better in some instances to simply re-define the adsr
//  (i'm thinking of a slow long adsr with many sub-sections being
//  scaled to become a very fast and short adsr.

//  the three member variables, attack_scale, decay_scale, and
//  release_scale, are multipliers for the current values of the adsr
//  to be scaled. a scale value of 1.0 will leave the section unscaled.

class adsr_scaler : public dobj::base
{
 public:
    adsr_scaler();
    ~adsr_scaler();

    // virtuals from dobj
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    errors::TYPE validate();

 private:
    adsr* padsr;
    double attack_scale;
    double decay_scale;
    double release_scale;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
