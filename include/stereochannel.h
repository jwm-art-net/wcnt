#ifndef STEREOCHANNEL_H
#define STEREOCHANNEL_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "paramnames.h"

// previously I'd used the same variables for each input and output
// instead of having seperate variables for input and output, keeping
// the run() code empty.
// did not work though, when the output was requested because it was
// of the way the connections are made, if a connection used get_out()
// before the inputs were set it would get 0 and thus think the
// connection could not be made.

class stereo_channel : public synthmod
{
public:
    stereo_channel(char const*);
    ~stereo_channel();
    void set_input_left(const short* l){ in_left = l;}
    void set_input_right(const short* r){ in_right = r;}
    const short* get_input_left(){ return in_left;}
    const short* get_input_right(){ return in_right;}
    const short* get_output_left(){ return &out_left;}
    const short* get_output_right(){ return &out_right;}
    // virtual funcs
    void run(){ out_left = *in_left; out_right = *in_right;}
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);

private:
    short out_left;
    short out_right;
    const short* in_left;
    const short* in_right;
    static int stereochannel_count;
};

#endif
