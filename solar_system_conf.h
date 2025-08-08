#ifndef SOLAR_SYSTEM_CONF_H
#define SOLAR_SYSTEM_CONF_H

#include "nbody_config.h"

extern Body sun;
extern Body mercury;
extern Body venus;
extern Body earth;
extern Body mars;
extern Body jupiter;
extern Body saturn;
extern Body uranus;
extern Body neptune;

#define N_BODIES 9

extern Body solar_system[N_BODIES];

extern void init_solar_system();

#define SOLAR_SYSTEM_PREFERRED_PLANET_RENDER_SCALE 10.0  // Large enough to see all planets clearly? Maybe 100?
#define SOLAR_SYSTEM_PREFERRED_POS_RENDER_SCALE 1//1e9//0.05 //10000000.0 * 100
#define SOLAR_SYSTEM_PREFERRED_DATA_TYPE DATA_TYPE_DOUBLE
extern const enum MassType solar_preferred_mass_type;
extern const enum SpaceType solar_preferred_space_type;
extern const enum TimeType solar_preferred_time_type;

#endif

