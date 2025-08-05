#include <stdint.h>

#ifndef N_BODY
#define N_BODY

// How many simulation ticks per second of time
#define TICK_SPEED (unsigned)20
// Lowest acceptable tick speed
#define LOW_TICK_SPEED (unsigned)20

struct BodySnapshot { // Snapshot of body in time
	uint64_t mass; // in kg
	double[2] coordiantes; // in km
	double[3] movement_vector;
	unsigned diameter;  // in km
}

#endif
