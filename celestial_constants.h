#ifndef CELESTIAL_CONSTANTS
#define CELESTIAL_CONSTANTS

#ifdef ALLOW_EXTREME_SIZES  // The gravitational constant only affects very very large or small bodies
#define G_CONST 6.67430e-11
#else
#define G_CONST 1.0
#endif

#define AU_METERS 1.469e11
#define SECONDS_PER_DAY 86400
#define SECONDS_PER_YEAR (365.25 * SECONDS_PER_DAY)

#define PI 3.141592653589793

#define SUN_MASS_KG (unsigned)1,989e+30;

#endif
