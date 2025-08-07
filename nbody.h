#ifndef NBODY_H
#define NBODY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "main.h"  // To get the configuration
#include "nbody_config.h"

// How many simulation ticks per second of time
#ifndef TICK_SPEED
#define TICK_SPEED 20
#endif
// Lowest tick speed
#ifndef LOW_TICK_SPEED
#define LOW_TICK_SPEED -20
#endif
// Highest tick speed
#ifndef HIGH_TICK_SPEED
#define HIGH_TICK_SPEED 40
#endif


// The simulation type
// #define USE_NEWTON_SIMULATION
// Enable GUI
// #define ENABLE_GUI


// Planet scale, 1 is normal size
#ifndef PLANET_RENDER_SCALE
#define PLANET_RENDER_SCALE 1
#endif

#ifndef POS_RENDER_SCALE
#define POS_RENDER_SCALE 1.0
#endif


#ifdef ENABLE_GUI
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#endif

int start_simulation(Body bodies[], size_t n_bodies, bool use_gui);
#endif
