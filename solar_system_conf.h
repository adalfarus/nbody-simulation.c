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

#define SS_LARGE_BODY_RENDER_SCALE 1e-3f//2.87e-6f
#define SS_LARGE_POS_RENDER_SCALE 1000.0f

#define SS_MIDDLE_BODY_RENDER_SCALE 1e-7f
#define SS_MIDDLE_POS_RENDER_SCALE 1e6f
#define SS_MIDDLE_MAX_BODY_RENDER_SCALE 10.0f
#define SS_MIDDLE_MIN_BODY_RENDER_SCALE 0.1f

#define SS_SMALL_BODY_RENDER_SCALE 1e8f
#define SS_SMALL_POS_RENDER_SCALE 1e7f
#define SS_SMALL_MAX_BODY_RENDER_SCALE 5.0f
#define SS_SMALL_MIN_BODY_RENDER_SCALE 0.01f

#define SOLAR_SYSTEM_PREFERRED_PLANET_RENDER_SCALE 2.87e-6f//1e-3f//10.0  // Large enough to see all planets clearly? Maybe 100?
#define SOLAR_SYSTEM_PREFERRED_POS_RENDER_SCALE 1000//1.0e7f / 1000//1000.0f//1//1e9//0.05 //10000000.0 * 100
#define SOLAR_SYSTEM_PREFERRED_DATA_TYPE DATA_TYPE_DOUBLE

extern const enum MassType solar_preferred_mass_type;
extern const enum SpaceType solar_preferred_space_type;
extern const enum TimeType solar_preferred_time_type;

#endif

