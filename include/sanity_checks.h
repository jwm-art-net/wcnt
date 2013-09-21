#ifndef SANITY_CHECKS_H
#define SANITY_CHECKS_H

#ifdef SANITY_CHECKS

class synthmod;

void sanity_checks();

bool mod_check_inputs(synthmod*);
bool mod_check_outputs(synthmod*);
bool mod_check_params(synthmod*);

#endif // SANITY_CHECKS


#endif
