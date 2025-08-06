#define ALLOW_EXTREME_BODIES
#define ENABLE_GUI

#include <stdbool.h>

#include "n_body.h"
#include "solar_system_initial.h"

int main(void) {
	init_solar_system();
	BodySnapshot bodies[2] = {sun, earth};
	return start_simulation(solar_system, N_BODIES, true);
	// return start_simulation(bodies, 2, true);
}
