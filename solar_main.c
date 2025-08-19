#include "solar_main.h"
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
			.enabled = true,
			.changeable = true, // Remove?
			.resizable = true,
			.window_width = 1000,
			.window_height = 700,
			.window_title = "N-Body Simulation",
			.target_fps = 60,
			.ui = (DisplayUIConfig) {
				.enabled = true,
				.font_size = 18, // TODO: Make smaller stuff relativ to this
			},
			.body_labels = (DisplayBodyLabelConfig) {
				.enabled = true,
				.font_size = 8,  // 18 Daynamic function?
				.show_name = true,
				.show_distance_to_camera = true,
				.show_distance_to_floating_origin = true, // TODO: Change from camera distance
				.show_speed = true,
				.show_cardinal_direction = true,
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
				.move_units_per_second = 25.0f,
				.speed_boost_mult = 6.0f,
				.zoom_units_per_second = 50.0f,
				.mouse_sensitivity = 0.003f
			},
			.vector_arrows = (DisplayVectorArrowConfig) {
				.enabled = true,
				.exaggeration_multiplier = 1.0f,
				.max_scalar = 10000.0f,
				.arrow_head_size_percent = 0.15f
			},
			.body_rendering = (DisplayBodyRenderConfig) {
				.enabled = true,
				.operation = SCALE_DIVIDE,
				.body_render_scale = SS_SMALL_BODY_RENDER_SCALE,
				.min_body_size = SS_SMALL_MIN_BODY_RENDER_SCALE,
				.max_body_size = SS_SMALL_MAX_BODY_RENDER_SCALE
			},
			.space_rendering = (DisplaySpaceRenderConfig) {
				.operation = SCALE_DIVIDE,
				.space_render_scale = SS_SMALL_POS_RENDER_SCALE
			},
			.skybox = (DisplaySkyboxConfig) {
				.enabled = true,
				.texture = "resources/skybox.png"
			},
			.grid_size = 10,
			.grid_density = 1.0f,

			.max_labels = 128,

			.disable_backface_culling = true
		},
		.min_body_velocity = 0.0,
		.max_body_velocity = 1000000.0,

		.base_physics_dt_sim = 1.0,
		.time_scale = 34.0 * 24.0,//1.0  One month per second
		.time_scale_min = 0.01,
		.time_scale_max = 1e12,

		.max_accumulator_backlog = 5000.0,

		.physics_tick_step_size_multiplier = 3600.0,//1.0,
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

