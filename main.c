#include "main.h"
#include "raylib.h" // For the camera enum

#include <stdbool.h>

int main(void) {
	init_solar_system();

	SimulationConfig cfg;
	DisplayConfig dc;
	
	cfg = (SimulationConfig){
	  .display = (DisplayConfig){
	    .enable = true,
	    .changeable = true,
	    .target_fps = 60,
	    .gui = (DisplayGUIConfig) {
	      .enable = true;
	      .font_size = 18,
	    },
	    .body_labels = (DisplayBodyLabelConfig){
	      .enable = true,
	      .font_size = 18,
	      .show_name = true,
	      .show_distance_to_floating_origin = true,
	      .show_speed = true,
	      .show_diameter = true,
	      .show_mass = true,
	    },
	    .unit_type_for_labels = SPACE_TYPE_KM,
	    .time_type_for_labels = TIME_TYPE_SECOND,
	    .camera = (DisplayCameraConfig){
	      .enable = true,
	      .changeable = true,
	      .initial_position = {0.0, 4.0, 10.0},
	      .initial_target = {0.0, 0.0, 0.0},
	      .up = { 0.0f, 1.0f, 0.0f },
	      .fovy = 45.0f,
	      .projection = CAMERA_PERSPECTIVE,
              .move_units_per_second = 1000.0,
              .mouse_sensitivity = 100.0f
	    },
	    .vec_config = (DisplayVectorArrowConfig){
	      .enable = true,
	      .exaggeration_multiplier = 1.0f,
	      .max_length_render = 10000.0f
	    },
	    .body_render_config = (){
	      .enable = true,
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
	  
	  //rest
	};
	
	cfg.display = dc;

	cfg.display.enable = true;
	cfg.display.changeable = true;
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

