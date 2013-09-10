#ifndef MODINPUT_H
#define MODINPUT_H

#ifndef BARE_MODULES

#include "synthmodule.h"

class modinput
{
 public:
	modinput(synthmod*, inputnames::IN_TYPE);
	~modinput(){};
	synthmod* getsynthmodule() { return (this != NULL) ? synthmodule : NULL; }
	synthmodnames::SYNTH_MOD_TYPE getmoduletype();
	inputnames::IN_TYPE getinputtype();
	IOCAT getinputcategory();

 private:
	synthmod* synthmodule;
	inputnames::IN_TYPE input_type;
};

#endif
#endif
