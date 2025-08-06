#include "nbody.h"
#include "nbody_config.h"
#include "nbody_types.h"
#include "celestial_constants.h"  // FOR G_CONST etc.

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>
#include <string.h> // For memset


void update_bodies(BodySnapshot bodies[], size_t n_bodies, double tick_speed) {
#ifdef USE_NEWTON_SIMULATION
	const double softening = 1e4;

	// Reset all accelerations
	for (size_t i = 0; i < n_bodies; i++) {
		bodies[i].acc[0] = bodies[i].acc[1] = bodies[i].acc[2] = 0.0;
	}

	// Compute gravitational accelerations
	//
	double potential = 0.0;
	double kinetic = 0.0;

	for (size_t i = 0; i < n_bodies; i++) {
		double v_sq = bodies[i].vel[0]*bodies[i].vel[0] +
			      bodies[i].vel[1]*bodies[i].vel[1] +
			      bodies[i].vel[2]*bodies[i].vel[2];
		kinetic += 0.5 * bodies[i].mass * v_sq;

		for (size_t j = 0; j < n_bodies; j++) {
			if (i == j) continue;

			double dx = bodies[j].pos[0] - bodies[i].pos[0];
			double dy = bodies[j].pos[1] - bodies[i].pos[1];
			double dz = bodies[j].pos[2] - bodies[i].pos[2];

			double dist_sq = dx * dx + dy * dy + dz * dz + softening * softening;
			double dist = sqrt(dist_sq);

			if (dist < 1e-5) continue; // Avoid a singularity
			potential -= G_CONST * bodies[i].mass * bodies[j].mass / dist;
			double force_mag = G_CONST * bodies[j].mass / (dist_sq * dist);  // G * m_j / |r|³

			bodies[i].acc[0] += force_mag * dx;
			bodies[i].acc[1] += force_mag * dy;
			bodies[i].acc[2] += force_mag * dz;
		}
	}

	printf("Energy: total=%e, kinetic=%e, potential=%e\n", kinetic + potential, kinetic, potential);

	// integrate velocity and position
	for (size_t i = 0; i < n_bodies; i++) {
		bodies[i].vel[0] += bodies[i].acc[0] * tick_speed;
		bodies[i].vel[1] += bodies[i].acc[1] * tick_speed;
		bodies[i].vel[2] += bodies[i].acc[2] * tick_speed;

		bodies[i].pos[0] += bodies[i].vel[0] * tick_speed;
		bodies[i].pos[1] += bodies[i].vel[1] * tick_speed;
		bodies[i].pos[2] += bodies[i].vel[2] * tick_speed;
	}
#else
	(void)bodies;
	(void)n_bodies;
	(void)tick_speed;
#endif
}

int gui_simulation(BodySnapshot bodies[], const char* names[], uint8_t colors[][3], size_t n_bodies) {
#ifdef ENABLE_GUI
	(void)names;  // How to display names next to planets and stars (bodies)?

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "N-Body Simulation");
	SetTargetFPS(60);

	Camera3D camera = {
		.position = { 0.0f, 200.0f, 500.0f },
		.target = { 0.0f, 0.0f, 0.0f },
		.up = { 0.0f, 1.0f, 0.0f },
		.fovy = 45.0f,
		.projection = CAMERA_PERSPECTIVE
	};

	double tick_speed = 1.0 / TICK_SPEED;  // Correct?

	while (!WindowShouldClose()) {
		UpdateCamera(&camera, CAMERA_FREE);

		if (1 == 0) {  // Only update if enough time has passed (n physics ticks per second)
			update_bodies(bodies, n_bodies, tick_speed);
		}

		                // Calculate system bounding box
                double min_x = bodies[0].pos[0], max_x = bodies[0].pos[0];
                double min_z = bodies[0].pos[2], max_z = bodies[0].pos[2];

                for (size_t i = 1; i < n_bodies; i++) {
                        if (bodies[i].pos[0] < min_x) min_x = bodies[i].pos[0];
                        if (bodies[i].pos[0] > max_x) max_x = bodies[i].pos[0];
                        if (bodies[i].pos[2] < min_z) min_z = bodies[i].pos[2];
                        if (bodies[i].pos[2] > max_z) max_z = bodies[i].pos[2];
                }

                double center_x = (min_x + max_x) / 2.0;
                double center_z = (min_z + max_z) / 2.0;
                double system_radius = fmax(max_x - min_x, max_z - min_z) / 2.0;

                // === Add buffer based on largest radius ===
                double max_radius = bodies[0].radius;
                for (size_t i = 1; i < n_bodies; i++) {
                        if (bodies[i].radius > max_radius)
                                max_radius = bodies[i].radius;
                }

                // Add a scaling factor to ensure the camera is far enough
                double camera_height = system_radius * 3.0 + max_radius * 5.0;

                camera.up = (Vector3){ 0.0f, 0.0f, -1.0f }; // North is -Z

		camera.position = (Vector3){ (float)center_x / POS_RENDER_SCALE, camera_height / POS_RENDER_SCALE, (float)center_z / POS_RENDER_SCALE };
                camera.target = (Vector3){ (float)center_x, 0.0f, (float)center_z };

                printf("POS %f %f %f\n", camera.position.x, camera.position.y, camera.position.z);
                printf("Loop %f %f %f\n", (float)bodies[0].pos[0], (float)bodies[0].pos[1], (float)bodies[0].pos[2]);
		printf("%f\n", POS_RENDER_SCALE);
                printf("TS: %f\n", tick_speed);

                // Drawing
                BeginDrawing();
                ClearBackground(BLACK);

                BeginMode3D(camera);

                for (size_t i = 0; i < n_bodies; i++) {
                        Vector3 pos = {
                                (float)bodies[i].pos[0] / POS_RENDER_SCALE,
                                (float)bodies[i].pos[1] / POS_RENDER_SCALE,
                                (float)bodies[i].pos[2] / POS_RENDER_SCALE
                        };
                        float radius = (float)(bodies[i].radius * 1e-9) * PLANET_RENDER_SCALE;
                        DrawSphere(pos, radius, colors[i]);
                }

                EndMode3D();

                DrawText(TextFormat("Tick speed: %.2f", tick_speed), 10, 10, 20, RAYWHITE);

                EndDrawing();
	}

	CloseWindow();
#else
	(void)bodies;
	(void)n_bodies;
	(void)names;
	(void)colors;
	printf("You cannot use the gui if you haven't enabled it during compilation!");
	return 1;
#endif
}


int start_simulation(Body bodies[], size_t n_bodies, bool use_gui) { 
	BodySnapshot body_snapshots[n_bodies];
	const char* names[n_bodies];
	uint8_t colors[n_bodies][3];

	for (size_t i = 0; i < n_bodies; i++) {
		names[i] = bodies[i].name;
		
		colors[i][0] = bodies[i].color[0];
		colors[i][1] = bodies[i].color[1];
		colors[i][2] = bodies[i].color[2];

		double conversion_mass = mass_conversion_table[bodies[i].mass_type][MASS_TYPE];
		double conversion_space = space_conversion_table[bodies[i].space_type][SPACE_TYPE];
		double conversion_time = time_conversion_table[bodies[i].time_type][TIME_TYPE];

		body_snapshots[i] = (BodySnapshot){
			.mass = (Real)(bodies[i].mass * conversion_mass),
			.radius = (Real)(bodies[i].radius * conversion_space),
			.pos = {
				(VectorT)(bodies[i].pos[0] * conversion_space),
				(VectorT)(bodies[i].pos[1] * conversion_space),
				(VectorT)(bodies[i].pos[2] * conversion_space)
			},
			.vel = {
				(VectorT)(bodies[i].vel[0] * conversion_space * conversion_time),
				(VectorT)(bodies[i].vel[1] * conversion_space * conversion_time),
				(VectorT)(bodies[i].vel[2] * conversion_space * conversion_time)
			},
			.acc = { 0, 0, 0 }
		};
	}
	
	if (use_gui) {
		return gui_simulation(body_snapshots, names, colors, n_bodies);
	} else {
		printf("Headless mode is not yet supported!\n");
		return 1;
	}

	//Vector3 offset = {
	//	.x = 0.0f,
	//	.y = 200.0f,
	//	.z = 500.0f
	//};

	//while (!WindowShouldClose()) {
		// Adjust tick speed
		// if (IsKeyPressed(KEY_UP)) tick_speed *= 2.0;
		// if (IsKeyPressed(KEY_DOWN)) tick_speed *= 0.5;
		// if (tick_speed < LOW_TICK_SPEED) tick_speed = LOW_TICK_SPEED;

		//if (IsKeyPressed(KEY_UP)) {
		//	camera.position.y *= 0.7;
		//	camera.position.z *= 0.7;
		//}
		//if (IsKeyPressed(KEY_DOWN)) {
		//	camera.position.y *= 1.7;
		//	camera.position.z *= 1.7;
		//}

		//Vector3 sun_pos = {
		//	(float)bodies[0].pos[0] / POS_RENDER_SCALE,
		//	(float)bodies[0].pos[1] / POS_RENDER_SCALE,
		//	(float)bodies[0].pos[2] / POS_RENDER_SCALE
		//};

		//(void)offset;
		//(void)sun_pos;

		//camera.position = Vector3Add(sun_pos, offset);
		//camera.target = sun_pos;
		// printf("%e\n", G_CONST);
	//}
}


