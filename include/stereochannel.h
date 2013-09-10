#ifndef STEREOCHANNEL_H
#define STEREOCHANNEL_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "paramnames.h"

class stereo_channel : public synthmod
{
public:
    stereo_channel(char const*);
    ~stereo_channel();
    void set_input_left(const short* l){ io_left = l;}
    void set_input_right(const short* r){ io_right = r;}
    const short* get_input_left(){ return io_left;}
    const short* get_input_right(){ return io_right;}
    const short* get_output_left(){ return io_left;}
    const short* get_output_right(){ return io_right;}
    // virtual funcs
    void run(){}; // check inputs/outputs for explanation of emptiness
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);

private:
    const short* io_left;
    const short* io_right;
    static int stereochannel_count;
};

#endif
