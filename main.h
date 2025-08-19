#ifndef MAIN_H
#define MAIN_H

#include "nbody_config.h"

// ---- Data layout for 5M dust ----
// pos=double (precision), vel/real=float (RAM + cache)
#define DATA_TYPE_REAL DT_UINT8
#define DATA_TYPE_POS  DT_UINT16
#define DATA_TYPE_VEC  DT_FLOAT

// If your code lets dust be massless/test-particles, keep SUPPORT_EXTREME_BODIES.
// (Otherwise ensure your stepper ignores dust-dust forces.)
#define SUPPORT_EXTREME_BODIES
#define INCLUDE_NEWTON_SIM
// #define INCLUDE_POST_NEWTONIAN_SIM   // disable PN for speed unless you truly need it

// Optional: comment out GUI at compile time to go fully headless
#define ENABLE_GUI

//#include "solar_system_conf.h"
#include "nbody_config.h"

// Use SI units for numerics (best with float vecs)
#define MASS_TYPE  MASS_TYPE_MG
#define SPACE_TYPE SPACE_TYPE_MM
#define TIME_TYPE  TIME_TYPE_SECOND

#include "nbody.h"

int main(void);

#endif // MAIN_H
