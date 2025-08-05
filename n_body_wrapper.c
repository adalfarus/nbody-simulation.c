#define ALLOW_EXTREME_BODIES
#define USE_NEWTON_SIMULATION
#define USE_GUI 1

#include "n_body.h"
#include "solar_system_initial.h"

int main(void) {
	return start_simulation(solar_system);
}
