#include "nbody_config.h"
#define DATA_TYPE_REAL DT_FLOAT
#define DATA_TYPE_POS  DT_INT64
#define DATA_TYPE_VEC  DT_FLOAT

#define ALLOW_EXTREME_BODIES
#define USE_NEWTON_SIMULATION  // Use einsteins for more accuracy
#define ENABLE_GUI

#define TICK_SPEED 1

#include <stdbool.h>

#include "solar_system_conf.h"
#include "nbody_config.h"

#define PLANET_RENDER_SCALE SS_SMALL_BODY_RENDER_SCALE
#define MAX_PLANET_RENDER_SCALE SS_SMALL_MAX_BODY_RENDER_SCALE//1000.0//500.0f//10.0
#define MIN_PLANET_RENDER_SCALE SS_SMALL_MIN_BODY_RENDER_SCALE//0.1//1.5e6f//0.1
#define POS_RENDER_SCALE SS_SMALL_POS_RENDER_SCALE
//#define DATA_TYPE DATA_TYPE_MIXED_64//SOLAR_SYSTEM_PREFERRED_DATA_TYPE  // Try DATA_TYPE_MIXED_64
#define MASS_TYPE MASS_TYPE_KG  // solar_preferred_mass_type
#define SPACE_TYPE SPACE_TYPE_KM//SPACE_TYPE_AU  //solar_preferred_space_type
#define TIME_TYPE TIME_TYPE_SECOND//TIME_TYPE_DAY  //solar_preferred_time_type

#include "nbody.h"

int main(void);

