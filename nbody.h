#ifndef NBODY_H
#define NBODY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h> // memcpy

#include "main.h"  // To get the configuration
#include "nbody_config.h"
#include "nbody_types.h"

#ifdef ENABLE_GUI
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#endif

typedef struct {
        bool enabled;
        uint8_t font_size;
} DisplayGUIConfig;

typedef struct {
	bool enabled;
	uint8_t font_size;
	bool show_name;
	bool show_distance_to_floating_origin;
	bool show_speed;
	bool show_diameter;
	bool show_mass;
} DisplayBodyLabelConfig;

typedef struct {
	bool enabled;
	bool changeable;
	double initial_position[3];
	double initial_target[3];
	float up[3];
	float fovy;
	int projection;
	double move_units_per_second;
	float mouse_sensitivity;
} DisplayCameraConfig;

typedef struct {
	bool enabled;
	float exaggeration_multiplier;
	float max_length_render;
} DisplayVectorArrowConfig;

typedef enum {
	SCALE_MULTIPLY,
	SCALE_DIVIDE
} Operation;

typedef struct {
	bool enabled;
	Operation operation;  // So you don't have to create small decimals or large numbers
	double body_render_scale;
	double min_body_size;
	double max_body_size;
} DisplayBodyRenderConfig;

typedef struct {
	bool enabled;
	Operation operation;  // So you don't have to create small decimals or large numbers
	double space_render_scale;
} DisplaySpaceRenderConfig;

typedef struct {
	bool enabled;
	bool changeable;
	uint8_t target_fps;
	DisplayGUIConfig gui;
	DisplayBodyLabelConfig body_labels;
	enum SpaceType unit_type_for_labels;
	enum TimeType time_type_for_labels;
	DisplayCameraConfig camera;
	DisplayVectorArrowConfig vec_config;
	DisplayBodyRenderConfig body_render_config;
} DisplayConfig;

typedef struct SimulationType  SimulationType;

typedef struct {
	DisplayConfig display;
	
	double min_body_velocity;  // 0.0 means ignore
	double max_body_velocity;

	// Time control
	double base_physics_dt_sim;
	double time_scale;
	double time_scale_min;
	double time_scale_max;

	double max_accumulator_backlog;

	// Integrator substepping
	double physics_tick_step_size_multiplier;
	double min_step_size_mult; // optional lower bound
	double max_step_size_mult; // clamp for stability (e.g. 3600 s)

	size_t start_time_sim;  // Start time point t (simulate up to that point before "starting")

	// History buffer (optional)
	size_t history_len;               // number of snapshots to keep
	bool keep_history;

	SimulationType* simulations;
	size_t simulations_count;
	size_t* simulation_order;  // Idx into simulations
	size_t simulation_order_count;
} SimulationConfig;

typedef struct {
	size_t count;
	BodySnapshot *buf;
} SnapshotFrame;

typedef struct {
	SnapshotFrame *frames;
	size_t capacity;
	size_t head;
	bool filled;
} HistoryRing;

static inline size_t history_last_index(const HistoryRing* hr) {
	return (hr->capacity == 0) ? 0 : (hr->filled ? (hr->head + hr->capacity - 1) % hr->capacity
	: (hr->head == 0 ? 0 : hr->head - 1));
}

static inline void history_push(HistoryRing *hr, const BodySnapshot *src, size_t nbodies) {
	if (!hr->frames || hr->capacity == 0) return; // tere is no history
	SnapshotFrame *f = &hr->frames[hr->head];
	memcpy(f->buf, src, nbodies * sizeof(BodySnapshot));
	hr->head = (hr->head + 1) % hr->capacity;
	if (hr->head == 0) hr->filled = true;
}

static inline BodySnapshot* history_clone_for_write(HistoryRing *hr, size_t nbodies) {
	if (!hr->frames || hr->capacity == 0) return NULL;
	size_t last = history_last_index(hr);
	size_t write = hr->head;
	memcpy(hr->frames[write].buf, hr->frames[last].buf, nbodies * sizeof(BodySnapshot));
	return hr->frames[write].buf; // caller must call history_commit() after stepping
}

static inline void history_commit(HistoryRing *hr) {
	if (!hr->frames || hr->capacity == 0) return;
	hr->head = (hr->head + 1) % hr->capacity;
	if (hr->head == 0) hr->filled = true;
}

typedef struct SimulationState  SimulationState;

typedef bool (*SimInitFn)(SimulationState* state);
typedef bool (*SimStepFn)(SimulationState* state, BodySnapshot *bodies, size_t nbodies, double dt_sim);
typedef void (*SimExitFn)(SimulationState* state);

struct SimulationState {
	size_t t_sim;  // Time point t
	double tick_accumulator;
	bool paused;
	size_t body_snapshots_idx;
	size_t simulation_step_fn_idx;
	size_t simulation_step_fn_count;

	SimStepFn* simulation_step_fns;

	size_t nbodies;
	const char** names;
	uint8_t (*colors)[3];
	VecT (*accelerations)[3];

	HistoryRing body_snapshots;  // Circular list of lists of body snapshots

	SimulationConfig config;
};

struct SimulationType {
	bool enabled;
	SimInitFn init;
	SimStepFn step;
	SimExitFn exit;
	void* data;  // per-sim user data (owned by the simulation type)
};

int start_simulation(const Body *bodies, size_t nbodies, size_t floating_origin_idx, SimulationConfig *cfg);


#ifdef INCLUDE_NEWTON_SIM
bool sim_newton_init(SimulationState* S);
bool sim_newton_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_newton_exit(SimulationState* S);
#endif

#ifdef INCLUDE_POST_NEWTONIAN_SIM
bool sim_post_newtonian_init(SimulationState* S);
bool sim_post_newtonian_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_post_newtonian_exit(SimulationState* S);
#endif

#ifdef INCLUDE_KEPLER_SIM
bool sim_kepler_init(SimulationState* S);
bool sim_kepler_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_kepler_exit(SimulationState* S);
#endif

#ifdef INCLUDE_KEPLER_PERTURBED_SIM
bool sim_kepler_perturbed_init(SimulationState* S);
bool sim_kepler_perturbed_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_kepler_perturbed_exit(SimulationState* S);
#endif

#ifdef INCLUDE_BARNES_HUT_SIM
bool sim_barnes_hut_init(SimulationState* S);
bool sim_barnes_hut_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_barnes_hut_exit(SimulationState* S);
#endif

#ifdef INCLUDE_FMM_SIM
bool sim_fmm_init(SimulationState* S);
bool sim_fmm_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_fmm_exit(SimulationState* S);
#endif

#ifdef INCLUDE_SPH_SIM
bool sim_sph_init(SimulationState* S);
bool sim_sph_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_sph_exit(SimulationState* S);
#endif

#ifdef INCLUDE_RELATIVISTIC_GEODESIC_SIM
bool sim_relativistic_geodesic_init(SimulationState* S);
bool sim_relativistic_geodesic_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_relativistic_geodesic_exit(SimulationState* S);
#endif

#ifdef INCLUDE_FULL_GR_SIM
bool sim_full_gr_init(SimulationState* S);
bool sim_full_gr_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_full_gr_exit(SimulationState* S);
#endif

#ifdef INCLUDE_EPHEMERIS_SIM
bool sim_ephemeris_init(SimulationState* S);
bool sim_ephemeris_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_ephemeris_exit(SimulationState* S);
#endif

#ifdef INCLUDE_X_SIM
bool sim_x_init(SimulationState* S);
bool sim_x_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim);
void sim_x_exit(SimulationState* S);
#endif

#endif
