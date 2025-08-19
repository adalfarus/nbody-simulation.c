#ifndef SOLAR_SYSTEM_CONF_H
#define SOLAR_SYSTEM_CONF_H

#include "nbody_config.h"

// Sun and planets
extern Body sun;
extern Body mercury;
extern Body venus;
extern Body earth;
extern Body mars;
extern Body jupiter;
extern Body saturn;
extern Body uranus;
extern Body neptune;
extern Body pluto;

// Earth system
extern Body moon;

// Jupiter major moons
extern Body io;
extern Body europa;
extern Body ganymede;
extern Body callisto;

// Saturn major moons
extern Body mimas;
extern Body enceladus;
extern Body tethys;
extern Body dione;
extern Body rhea;
extern Body titan;
extern Body iapetus;

// Uranus major moons
extern Body miranda;
extern Body ariel;
extern Body umbriel;
extern Body titania;
extern Body oberon;

// Neptune major moons
extern Body triton;
extern Body proteus;

// Pluto system
extern Body charon;
extern Body nix;
extern Body hydra;
extern Body kerberos;
extern Body styx;

#define N_BODIES 37

extern Body solar_system[N_BODIES];

extern void init_solar_system();

//#define SS_LARGE_BODY_RENDER_SCALE 1e-3f//2.87e-6f
//#define SS_LARGE_POS_RENDER_SCALE 1000.0f

#define SS_MIDDLE_BODY_RENDER_SCALE 19531.25f
#define SS_MIDDLE_POS_RENDER_SCALE 625000.0f
#define SS_MIDDLE_MAX_BODY_RENDER_SCALE 10.0f
#define SS_MIDDLE_MIN_BODY_RENDER_SCALE 0.01f

#define SS_SMALL_BODY_RENDER_SCALE 1e7f
#define SS_SMALL_POS_RENDER_SCALE 1e7f
#define SS_SMALL_MAX_BODY_RENDER_SCALE 5.0f
#define SS_SMALL_MIN_BODY_RENDER_SCALE 0.001f

#define SS_TINY_BODY_RENDER_SCALE 1e7f
#define SS_TINY_POS_RENDER_SCALE 160000000.0f
#define SS_TINY_MAX_BODY_RENDER_SCALE 5.0f
#define SS_TINY_MIN_BODY_RENDER_SCALE 0.001f

//#define SOLAR_SYSTEM_PREFERRED_PLANET_RENDER_SCALE 2.87e-6f//1e-3f//10.0  // Large enough to see all planets clearly? Maybe 100?
//#define SOLAR_SYSTEM_PREFERRED_POS_RENDER_SCALE 1000//1.0e7f / 1000//1000.0f//1//1e9//0.05 //10000000.0 * 100
#define SS_PREFERRED_DATA_TYPE DT_DOUBLE

extern const enum MassType solar_preferred_mass_type;
extern const enum SpaceType solar_preferred_space_type;
extern const enum TimeType solar_preferred_time_type;

#endif

