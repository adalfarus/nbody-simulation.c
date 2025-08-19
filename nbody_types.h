#ifndef NBODY_TYPES_H
#define NBODY_TYPES_H

#include <stdint.h>
#include <inttypes.h>

#if DATA_TYPE_REAL == DT_DOUBLE
typedef double Real;
#define PRIReal ".17g"
// Any macro functions

#elif DATA_TYPE_REAL == DT_FLOAT
typedef float Real;
#define PRIReal "f"
// Any macro functions

#elif DATA_TYPE_REAL == DT_UINT64
typedef uint64_t Real;
#define PRIReal PRIu64
// Any macro functions

#elif DATA_TYPE_REAL == DT_UINT32
typedef uint32_t Real;
#define PRIReal PRIu32
// Any macro functions

#elif DATA_TYPE_REAL == DT_UINT16
typedef uint16_t Real;
#define PRIReal PRIu16
// Any macro functions

#elif DATA_TYPE_REAL == DT_UINT8
typedef uint8_t Real;
#define PRIReal PRIu8
// Any macro functions

#else
#error "No real representation defined! Define one using DATA_TYPE_REAL. DOUBLE, FLOAT or unsiged fixed point types are allowed."
#endif

#if DATA_TYPE_POS == DT_DOUBLE
typedef double PosT;
#define PRIPosT ".17g"
// Any macro functions

#elif DATA_TYPE_POS == DT_FLOAT
typedef float PosT;
#define PRIPosT "f"
// Any macro functions

#elif DATA_TYPE_POS == DT_UINT64
typedef uint64_t PosT;
#define PRIPosT PRIu64
// Any macro functions

#elif DATA_TYPE_POS == DT_INT64
typedef int64_t PosT;
#define PRIPosT PRIi64
// Any macro functions

#elif DATA_TYPE_POS == DT_UINT32
typedef uint32_t PosT;
#define PRIPosT PRIu32
// Any macro functions

#elif DATA_TYPE_POS == DT_INT32
typedef int32_t PosT;
#define PRIPosT PRIi32
// Any macro functions

#elif DATA_TYPE_POS == DT_UINT16
typedef uint16_t PosT;
#define PRIPosT PRIu16
// Any macro functions

#elif DATA_TYPE_POS == DT_INT16
typedef int16_t PosT;
#define PRIPosT PRIi16
// Any macro functions

#elif DATA_TYPE_POS == DT_UINT8
typedef uint8_t PosT;
#define PRIPosT PRIu8
// Any macro functions

#elif DATA_TYPE_POS == DT_INT8
typedef int8_t PosT;
#define PRIPosT PRIi8
// Any macro functions

#else
#error "No position type representation defined! Define one using DATA_TYPE_POS."
#endif

#if DATA_TYPE_VEC == DT_DOUBLE
typedef double VecT;
#define PRIVecT ".17g"
// Any macro functions
#define VEC_SQRT(x) sqrt(x)

#elif DATA_TYPE_VEC == DT_FLOAT
typedef float VecT;
#define PRIVecT "f"
// Any macro functions
#define VEC_SQRT(x) sqrtf(x)

#else
#error "No vector type representation defined! Define one using DATA_TYPE_VEC. DOUBLE, and FLOAT types are allowed."
#endif

typedef struct {
	Real mass;
	Real radius;

	PosT pos[3];

	VecT vel[3];
	// VecT acc[3];  // Remove, this way we only store 3 VecT's for the current simulation step instead of for all cached ones
} BodySnapshot;

#endif
