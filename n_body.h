#ifndef N_BODY_H
#define N_BODY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


// How many simulation ticks per second of time
#define TICK_SPEED (uint8_t)20
// Lowest acceptable tick speed
#define LOW_TICK_SPEED (uint8_t)20


typedef struct { // Snapshot of body in time
	// uint64_t mass;  // in kg
	// uint64_t radius;  // in AU
	double mass;
	double radius;
	double pos[3];  // in AU
	double vel[3];  // AU/day
	double acc[3];  // AU/day²
} BodySnapshot;

int start_simulation(BodySnapshot bodies[]);

#endif
