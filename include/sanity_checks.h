#ifndef SANITY_CHECKS_H
#define SANITY_CHECKS_H

#ifdef SANITY_CHECKS
#include "synthmod.h"

// these SANITY_CHECKS are pretty much legacy code
// from before I moved over to the more sophisticated
// (and horribly complicated, undocumented, and lacking
// comments) way of handling inputs, parameters, and
// data objects using the ui_* set of classes. this allows
// for the simplest of use cases to be expressed simply
// without specifying that which is beyond the user
// requirements. or put another way, it takes the onus
// from the user to propagate default values for features
// they are not using. for example, a clock set to a static frequency
// without modulation - it's input is off and
// the modulation amount is zero by default no
// more insistence you set it manually).
// wow. what a sentence! the ui_ set of classes
// were written at my coding peak (always as a hobbyist),
// i found i was at a limit, there were not enough hours
// in the day (or night) for me to improve further.
// my life then changed and i stopped it all, the coding
// hadn't got me where i wanted to be, and where i ended
// didn't need it... and i wasn't as good as i thought i
// was. limited, big ommissions, and i care not for
// writing software for business. it was always driven
// by my own special interests, and a motivation to be
// better, and acceptance from others i looked up to.
// but that never came. a runt. black sheep. cast out.
// keep at arms length, don't let in,
// -jwm april 2026

void sanity_checks();

bool mod_check_inputs(synthmod::base*);
bool mod_check_outputs(synthmod::base*);
bool mod_check_params(synthmod::base*);

#endif // SANITY_CHECKS


#endif
