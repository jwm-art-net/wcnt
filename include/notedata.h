#ifndef NOTEDATA_H
#define NOTEDATA_H

#include <string.h>

#include "linkedlist.h"
#include "conversions.h"
#include "dobjparamlist.h"

// note_data
// there used to be a whole load of comments with regards to note length
// and position values, but since quarter_value is no longer hard coded,
// and now defined by the user for each riff, it seemed pointless having
// the comments regarding conversions..

class note_data : public dobj
{
public:
    note_data();
    note_data(const char* name, double len, double pos, double vel);
    ~note_data();
    void set_name(const char * n);
    void set_length(double l) { length = l; }
    void set_position(double p) { position = p; }
    void set_velocity(double v) { velocity = v; }
    const char* get_name() { return (this == NULL) ? NULL : notename; }
    double get_length() { return (this == NULL) ? 0 : length; }
    double get_position() { return (this == NULL) ? 0 : position; }
    double get_velocity() { return (this == NULL) ? 0.00 : velocity; }
    // virtuals from dobj
    char const* get_username(){ return notename;}
#ifndef BARE_MODULES
    // virtuals from dobj
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE);
    stockerrs::ERR_TYPE validate();
#endif
private:
    char notename[NOTE_ARRAY_SIZE];
    double length;
    double position;
    double velocity;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
