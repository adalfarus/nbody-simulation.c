#define ALLOW_EXTREME_BODIES
#define USE_NEWTON_SIMULATION  // Use einsteins for more accuracy
#define ENABLE_GUI

#define TICK_SPEED 40

#include <stdbool.h>

#include "solar_system_conf.h"
#include "nbody_config.h"

#define PLANET_RENDER_SCALE SOLAR_SYSTEM_PREFERRED_PLANET_RENDER_SCALE
#define POS_RENDER_SCALE SOLAR_SYSTEM_PREFERRED_POS_RENDER_SCALE
#define DATA_TYPE SOLAR_SYSTEM_PREFERRED_DATA_TYPE  // Try DATA_TYPE_MIXED_64
#define MASS_TYPE MASS_TYPE_KG  // solar_preferred_mass_type
#define SPACE_TYPE SPACE_TYPE_AU  //solar_preferred_space_type
#define TIME_TYPE TIME_TYPE_DAY  //solar_preferred_time_type

#include "nbody.h"

int main(void) {
	init_solar_system();
	Body bodies[2] = {sun, earth};
	start_simulation(solar_system, N_BODIES, true);
	return start_simulation(bodies, 2, true);
}

