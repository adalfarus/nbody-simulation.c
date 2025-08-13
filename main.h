#include "nbody_config.h"
#define DATA_TYPE_REAL DT_FLOAT
#define DATA_TYPE_POS  DT_INT64
#define DATA_TYPE_VEC  DT_FLOAT

#define SUPPORT_EXTREME_BODIES
#define INCLUDE_NEWTON_SIM
#define INCLUDE_POST_NEWTONIAN_SIM
#define ENABLE_GUI

#include "solar_system_conf.h"
#include "nbody_config.h"

#define MASS_TYPE MASS_TYPE_KG  // solar_preferred_mass_type
#define SPACE_TYPE SPACE_TYPE_KM //SPACE_TYPE_AU  //solar_preferred_space_type
#define TIME_TYPE TIME_TYPE_SECOND //TIME_TYPE_DAY  //solar_preferred_time_type

#include "nbody.h"

int main(void);

