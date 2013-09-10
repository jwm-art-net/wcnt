#ifndef DTR_H
#define DTR_H

#include <math.h>

// mingw32 and visual c++ 6 don't appear to have this:
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

// multiply degrees by DTR to get radians for
// use with maths functions: sin, cos, etc
// the value for PI above was 'sourced' from
// windows98 calculator if you're interested.

const double DTR = M_PI / 180;

#endif // DTR_H
