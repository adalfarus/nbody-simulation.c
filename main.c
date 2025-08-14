#include "main.h"
#include "raylib.h" // For the camera enum

#include <stdbool.h>

int main(void) {
	init_solar_system();

	static SimulationType sims[] = {
		{ .enabled = true, .init = sim_newton_init, .step = sim_newton_step, .exit = sim_newton_exit, .data = NULL },
	};

	static size_t sim_order[] = { 0 };

	SimulationConfig cfg = (SimulationConfig) {
		.display = (DisplayConfig) {
			.enabled = false,
			.changeable = true,
			.target_fps = 60,
			.gui = (DisplayGUIConfig) {
				.enabled = true,
				.font_size = 18,
			},
			.body_labels = (DisplayBodyLabelConfig) {
				.enabled = true,
				.font_size = 18,
				.show_name = true,
				.show_distance_to_floating_origin = true,
				.show_speed = true,
				.show_diameter = true,
				.show_mass = true,
			},
			.unit_type_for_labels = SPACE_TYPE_KM,
			.time_type_for_labels = TIME_TYPE_SECOND,
			.camera = (DisplayCameraConfig) {
				.enabled = true,
				.changeable = true,
				.initial_position = { 0.0, 4.0, 10.0 },
				.initial_target = { 0.0, 0.0, 0.0 },
				.up = { 0.0f, 1.0f, 0.0f },
				.fovy = 45.0f,
				.projection = CAMERA_PERSPECTIVE,
				.move_units_per_second = 1000.0,
				.mouse_sensitivity = 100.0f
			},
			.vec_config = (DisplayVectorArrowConfig) {
				.enabled = true,
				.exaggeration_multiplier = 1.0f,
				.max_length_render = 10000.0f
			},
			.body_render_config = (DisplayBodyRenderConfig) {
				.enabled = true,
				.operation = SCALE_DIVIDE,
				.body_render_scale = SS_SMALL_BODY_RENDER_SCALE,
				.min_body_size = SS_SMALL_MIN_BODY_RENDER_SCALE,
				.max_body_size = SS_SMALL_MAX_BODY_RENDER_SCALE
			}
		},
		.min_body_velocity = 0.0,
		.max_body_velocity = 1000000.0,

		.base_physics_dt_sim = 1.0,
		.time_scale = 1.0,
		.time_scale_min = 0.01,
		.time_scale_max = 1e8,

		.max_accumulator_backlog = 5.0,

		.physics_tick_step_size_multiplier = 1.0,
		.min_step_size_mult = 0.0,
		.max_step_size_mult = 3600.0,
		.start_time_sim = 0,

		.history_len = 100,
		.keep_history = true,
		.simulations = sims,
		.simulations_count = sizeof(sims)/sizeof(sims[0]),
		.simulation_order = sim_order,
		.simulation_order_count = sizeof(sim_order)/sizeof(sim_order[0]),
	};

	Body bodies[2] = { sun, earth };
	start_simulation(bodies, 2, 0, &cfg);
	return start_simulation(solar_system, N_BODIES, 0, &cfg);
}
