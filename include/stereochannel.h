#ifndef STEREOCHANNEL_H
#define STEREOCHANNEL_H

#include "synthmodule.h"

/*
// previously I'd used the same variables for each input and output
// instead of having seperate variables for input and output, keeping
// the run() code empty.
// did not work though, when the output was requested because it was
// of the way the connections are made, if a connection used get_out()
// before the inputs were set it would get 0 and thus think the
// connection could not be made.
*/

class stereo_channel : public synthmod
{
public:
    stereo_channel(char const*);
    ~stereo_channel();
    // these two used by stereomixer:
    const double* get_output_left() { return &out_left;  }
    const double* get_output_right(){ return &out_right; }
    void run(){ out_left = *in_left; out_right = *in_right;}
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;

private:
    double out_left;
    double out_right;
    const double* in_left;
    const double* in_right;
};

#endif
