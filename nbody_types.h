#ifndef NBODY_TYPES_H
#define NBODY_TYPES_H

#include <stdint.h>

#if DATA_TYPE == DATA_TYPE_DOUBLE
typedef double Real;
typedef double VectorT;
#elif DATA_TYPE == DATA_TYPE_FLOAT
typedef float Real;
typedef float VectorT;
#elif DATA_TYPE == DATA_TYPE_MIXED_64
typedef uint64_t Real;
typedef float VectorT;
#elif DATA_TYPE == DATA_TYPE_MIXED_32
typedef uint32_t Real;
typedef float VectorT;
#elif DATA_TYPE == DATA_TYPE_MIXED_16
typedef uint16_t Real;
typedef float VectorT;
#elif DATA_TYPE == DATA_TYPE_MIXED_8
typedef uint8_t Real;
typedef float VectorT;
#else
#error "No body representation defined! Define one using DATA_TYPE."
#endif

typedef struct {
	Real mass;
	Real radius;

	VectorT pos[3];
	VectorT vel[3];
	VectorT acc[3];
} BodySnapshot;

#endif

