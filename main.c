#include "main.h"

int main(void) {
	init_solar_system();
	// Body bodies[2] = {sun, earth};
	// start_simulation(bodies, 2, true);
	start_simulation(solar_system, N_BODIES, true);
}

