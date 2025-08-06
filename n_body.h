#ifndef N_BODY_H
#define N_BODY_H

#include "celestial_constants.h"  // For G_CONST etc.

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


// How many simulation ticks per second of time
#define TICK_SPEED 200000
// Lowest acceptable tick speed
// #define LOW_TICK_SPEED (uint8_t)20
// The simulation type
#define USE_NEWTON_SIMULATION
// Enable GUI
#define ENABLE_GUI
// Planet scale, 1 is normal size
#define PLANET_RENDER_SCALE (uint8_t)100

// #define POS_RENDER_SCALE 0.00000001//(uint8_t)1e9
//#define POS_RENDER_SCALE 1e8f
// #define POS_RENDER_SCALE 260000000.0
#define POS_RENDER_SCALE 10000000.0


// #ifdef ENABLE_GUI
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
// #endif

typedef struct { // Snapshot of body in time
	// uint64_t mass;  // in kg
	// uint64_t radius;  // in AU
	double mass;
	double radius;
	double pos[3];  // in AU
	double vel[3];  // AU/day
	double acc[3];  // AU/day²
} BodySnapshot;

typedef enum {
	NEWTON
} SimulationType;

int start_simulation(BodySnapshot bodies[], size_t n_bodies, bool use_gui);

#endif
