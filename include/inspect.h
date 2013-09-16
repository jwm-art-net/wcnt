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

class inspect : public synthmod
{
public:
    inspect(char const*);
    ~inspect();
    void run();
    void const* get_out(outputnames::OUT_TYPE) { return 0; }
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    /* inputs */
    double const* in_df;            /* double float*/
    STATUS const* in_df_trig;
    short  const* in_si;            /* short int */
    STATUS const* in_si_trig;
    unsigned long const* in_ul;     /* unisnged long */
    STATUS const* in_ul_trig;
    STATUS const* in_st;            /* state */
    STATUS const* in_st_trig;
    char  const** in_str;           /* string */
    STATUS const* in_str_trig;

    /* params */
    char* df_msg;
    char* si_msg;
    char* ul_msg;
    char* st_msg;
    char* str_msg;

    void set_message(char const* msg_from, char** msg_to);
    void create_params();
    
};

#endif
