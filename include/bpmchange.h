#ifndef BPMCHANGE_H
#define BPMCHANGE_H

#include "dobj.h"

// tempo changes are now relative.
// so when tobpm is 10, the tempo will increase by 10 bpm.
// obvious: negative tempo decrease speed

class bpmchange : public dobj::base
{
public:
    bpmchange();
    bpmchange(wcint_t bar, double bpm);
    void set_bar(wcint_t bar){atbar = bar;}
    void set_bpm(double bpm){tobpm = bpm;}
    wcint_t get_bar() const { return atbar;}
    double get_bpm() const { return tobpm;}
    errors::TYPE validate();
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE pt) const;

private:
    wcint_t atbar;
    double tobpm;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
