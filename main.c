#include "main.h"

#include <stdbool.h>

int main(void) {
	init_solar_system();

	SimulationConfig cfg;

	cfg.display.enable = true;
	cfg.display.space.enable = true;
	cfg.display.space.space_render_scale = 1.0/1e9;

	C->base_physics_dt_sim = 1.0;    // 1 sim second per physics step
	C->time_scale = 3600.0;          // 1 real second = 1 sim hour
	C->time_scale_min = 1e-3;
	C->time_scale_max = 1e7;
	C->max_dt_sim = 3600.0;          // clamp per step to 1 hour
	C->min_dt_sim = 0.0;

	sims[0] = (SimulationType){ .enable=true, .init=sim_newton_init, .step=sim_newton_step, .exit=sim_newton_exit, .data=NULL };
	C->sims = sims; C->sims_count = 1;
	order[0] = 0;
	C->sim_order = order;

	// nbody_make_default_config(&cfg);
	cfg.mass_out = MASS_TYPE_KG;
	cfg.space_out = SPACE_TYPE_KM;
	cfg.time_out = TIME_TYPE_SECOND;
	cfg.simulation_seconds_per_second = 86400.0;

	// Body bodies[2] = {sun, earth};
	// start_simulation(bodies, 2, true);
	return start_simulation(solar_system, N_BODIES, 0, &cfg);
}

