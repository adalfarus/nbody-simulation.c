#include "main.h"
#include "raylib.h" // Only needed if you keep display enabled

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#define DUST_COUNT   5000000u          // 5 million
#define DUST_COUNT 1000u
#define DUST_NAME    "dust"
#define DUST_RADIUS  50.0              // meters (visual placeholder; simulation should ignore)
#define DUST_COLOR_R 200
#define DUST_COLOR_G 200
#define DUST_COLOR_B 255

// Disk geometry (meters)
#define DUST_RMIN    (5.0e9)           // 5e9 m   ~ 0.033 AU
#define DUST_RMAX    (4.5e11)          // 4.5e11 m ~ 3.0 AU
#define DUST_Z_STD   (1.0e8)           // vertical sigma (100,000 km)

// Velocity jitter
#define DUST_TANGENTIAL_JITTER  0.02    // ±2 % of vcirc
#define DUST_RADIAL_JITTER_RMS  50.0    // m/s

// Sun GM (m^3/s^2). If you already define this elsewhere, use that.
static const double GM_SUN = 1.32712440018e20;

// RNG helpers (fast, deterministic)
static inline unsigned int xorshift32(unsigned int *s) {
    unsigned int x = *s;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    return *s = x;
}
static inline double u01(unsigned int *s) { return (xorshift32(s) / 4294967296.0); }
static inline double gauss01(unsigned int *s) { // Box–Muller
    double u = fmax(u01(s), 1e-12), v = u01(s);
    return sqrt(-2.0*log(u)) * cos(2.0*M_PI*v);
}

// Create a single shared immutable name to save RAM
static const char *get_dust_name(void) {
    static const char* n = DUST_NAME;
    return n;
}

// Generate a Keplerian disk of N test particles around origin
// pos in meters, vel in m/s.
static Body* generate_dust(size_t N) {
    Body* dust = (Body*)malloc(sizeof(Body) * N);
    if (!dust) {
        fprintf(stderr, "ERROR: Failed to alloc %zu dust bodies (~%.1f MB)\n",
                N, (sizeof(Body)*N) / (1024.0*1024.0));
        return NULL;
    }

    unsigned int seed = 0xC0FFEEu; // fixed seed for reproducibility
    const char* sharedName = get_dust_name();

    for (size_t i = 0; i < N; ++i) {
        // Radius with area-uniform sampling: r = sqrt(lerp(Rmin^2, Rmax^2))
        double u = u01(&seed);
        double r = sqrt((1.0 - u) * (DUST_RMIN*DUST_RMIN) + u * (DUST_RMAX*DUST_RMAX));
        double th = u01(&seed) * 2.0 * M_PI;

        // Position (meters)
        double x = r * cos(th);
        double y = DUST_Z_STD * gauss01(&seed);     // thin vertical thickness
        double z = r * sin(th);

        // Keplerian tangential speed
        double v_circ = sqrt(GM_SUN / fmax(r, 1.0));
        // small velocity jitter
        double vt = v_circ * (1.0 + (u01(&seed)*2.0 - 1.0) * DUST_TANGENTIAL_JITTER);
        double vr = gauss01(&seed) * DUST_RADIAL_JITTER_RMS;

        // Tangential and radial unit vectors in plane
        double ux = -sin(th), uz = cos(th); // tangential direction
        double rx =  cos(th), rz = sin(th); // radial direction

        // Velocity components (no vertical drift)
        double vx = vt*ux + vr*rx;
        double vy = 0.0;
        double vz = vt*uz + vr*rz;

        Body b;
        b.name = sharedName;                   // shared const pointer
        b.color[0] = DUST_COLOR_R;
        b.color[1] = DUST_COLOR_G;
        b.color[2] = DUST_COLOR_B;

        b.mass_type  = MASS_TYPE_KG;
        b.space_type = SPACE_TYPE_M;
        b.time_type  = TIME_TYPE_SECOND;

        b.mass   = 0.01;                        // <<< test particle; must NOT attract
        b.radius = DUST_RADIUS;                // visual only (engine may clamp)

        b.pos[0] = x; b.pos[1] = y; b.pos[2] = z;
        b.vel[0] = vx; b.vel[1] = vy; b.vel[2] = vz;

        dust[i] = b;
    }

    return dust;
}

int main(void) {
    Body* dust = generate_dust(DUST_COUNT);
    if (!dust) return 1;

    static SimulationType sims[] = {
        { .enabled = true, .init = sim_newton_init, .step = sim_newton_step, .exit = sim_newton_exit, .data = NULL },
    };
    static size_t sim_order[] = { 0 };

    SimulationConfig cfg = (SimulationConfig) {
        .display = (DisplayConfig) {
            .enabled = true,
            .changeable = false,
            .resizable = false,
            .window_width = 1280,
            .window_height = 720,
            .window_title = "N-Body (Dust + Camera)",
            .target_fps = 30,  // OK since display is enabled

            .ui = (DisplayUIConfig){ .enabled = true, .font_size = 18 },

            // KEEP THESE DISABLED (expensive):
            .body_labels = (DisplayBodyLabelConfig) {
                .enabled = true,
                .font_size = 8, // 18 Daynamic function?
                .show_name = true,
                .show_distance_to_camera = true,
                .show_distance_to_floating_origin = true, // TODO: Change from camera distance
                .show_speed = true,
                .show_cardinal_direction = true,
                .show_diameter = true,
                .show_mass = true,
            },

            .unit_type_for_labels = SPACE_TYPE_M,
            .time_type_for_labels = TIME_TYPE_SECOND,

            .camera = (DisplayCameraConfig){
                .enabled = true,
                .changeable = true,
                .initial_position = { 0.0f, 2.0f, 8.0f },
                .initial_target   = { 0.0f, 0.0f, 0.0f },
                .up = { 0.0f, 1.0f, 0.0f },
                .fovy = 45.0f,
                .projection = CAMERA_PERSPECTIVE,
                .move_units_per_second = 50.0f,   // in *view* units
                .speed_boost_mult = 6.0f,
                .zoom_units_per_second = 100.0f,
                .mouse_sensitivity = 0.003f
            },

            .vector_arrows = (DisplayVectorArrowConfig){ .enabled = false, .exaggeration_multiplier = 0.0f, .max_scalar = 0.0f, .arrow_head_size_percent = 0.0f },

            // Concrete numeric values — tuned for meters → view scaling
            .body_rendering = (DisplayBodyRenderConfig){
                .enabled = true,
                .operation = SCALE_DIVIDE,
                // If your renderer computes displayRadius = physicalRadius / body_render_scale:
                // Set this so ~1000 km bodies show up a few view-units wide.
                .body_render_scale = 5.0e5f,         // meters per view-unit (e.g., 500 km per view unit)
                .min_body_size = 1.0f,               // min view-units for tiny bodies
                .max_body_size = 30.0f               // cap big bodies in view-units
            },

            .space_rendering = (DisplaySpaceRenderConfig){
                .operation = SCALE_DIVIDE,
                // World positions in meters → divide by 1e6 to get megameters in view space.
                .space_render_scale = 1.0e6f         // 1,000,000 m per view-unit
            },

            .skybox = (DisplaySkyboxConfig){ .enabled = false, .texture = "resources/skybox.png" },
            .grid_size = 0,
            .grid_density = 0.0f,
            .max_labels = DUST_COUNT,
            .disable_backface_culling = false
        },

        // ----- PHYSICS / TIMESTEPPING -----
        // Use small, stable base dt; scale with time_scale for speed.
        .base_physics_dt_sim = 0.25,        // seconds-per-step base
        .time_scale = 1.0,                  // 1 sim sec per real sec; raise carefully after validating stability
        .time_scale_min = 1e-4,
        .time_scale_max = 1e9,

        // Avoid the simulation “spiraling” if rendering stalls
        .max_accumulator_backlog = 0.5,     // seconds; keep tight in headless mode

        // If your integrator supports substepping multiplier:
        .physics_tick_step_size_multiplier = 1.0,
        .min_step_size_mult = 1.0,
        .max_step_size_mult = 1.0,

        .start_time_sim = 0,

        // ----- HOUSEKEEPING -----
        .history_len = 1,                   // no trails/history
        .keep_history = true,

        // Clamp body velocity if you have unstable spawns (optional)
        .min_body_velocity = 0.0,
        .max_body_velocity = 3.0e5f,        // ~orbital-ish speeds; tune to your scene

        .simulations = sims,
        .simulations_count = sizeof(sims)/sizeof(sims[0]),
        .simulation_order = sim_order,
        .simulation_order_count = sizeof(sim_order)/sizeof(sim_order[0]),
    };

    // NOTE: For 5M dust, ensure your stepper treats dust as test particles
    // (no dust-dust forces). Spawn them with zero/very small mass.
    // start_simulation(dust_array, N_DUST, 0, &cfg);
    // For now, this keeps the massive solar system primaries:
    printf("TRunning sim\n");
    int rc = start_simulation(dust, DUST_COUNT, 0, &cfg);
    free(dust);
    return rc;
}
