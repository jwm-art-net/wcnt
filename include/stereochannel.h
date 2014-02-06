#ifndef STEREOCHANNEL_H
#define STEREOCHANNEL_H

#include "synthmod.h"

/*
// previously I'd used the same variables for each input and output
// instead of having seperate variables for input and output, keeping
// the run() code empty.
// did not work though, when the output was requested because it was
// of the way the connections are made, if a connection used get_out()
// before the inputs were set it would get 0 and thus think the
// connection could not be made.
*/

class stereo_channel : public synthmod::base
{
public:
    stereo_channel(const char*);
    ~stereo_channel();
    // these two used by stereomixer:
    void run(){};
    const double* get_output_left() const  { return io_left;  }
    const double* get_output_right() const { return io_right; }
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;

private:
    const double* io_left;
    const double* io_right;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
