#ifndef INSPECT_H
#define INSPECT_H

#include "synthmod.h"

/*
//  inspect
//  
//  contains an input for each of the input/output types along with
//  a corresponding trigger input. when the corresponding trigger
//  input triggers, the message for that type is displayed, along
//  with the value of the input.

*/

class inspect : public synthmod::base
{
 public:
    inspect(const char*);
    ~inspect();
    void run();
    const void* get_out(output::TYPE) { return 0; }
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    /* inputs */
    double const*   in_df;            /* double float*/
    STATUS const*   in_df_trig;
    wcint_t const*  in_si;            /* int (previously short) */
    STATUS const*   in_si_trig;
    samp_t const*   in_ul;            /* samp_t (previously unsigned long)*/
    STATUS const*   in_ul_trig;
    STATUS const*   in_st;            /* state */
    STATUS const*   in_st_trig;
    char  const**   in_str;           /* string */
    STATUS const*   in_str_trig;

    /* params */
    char* df_msg;
    char* si_msg;
    char* ul_msg;
    char* st_msg;
    char* str_msg;

    void set_message(const char* msg_from, char** msg_to);
    void register_ui();
};

#endif
