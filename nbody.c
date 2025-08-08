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

#define GLSL_VERSION            330

int gui_simulation(BodySnapshot bodies[], const char* names[], uint8_t colors[][3], size_t n_bodies) {
#ifdef ENABLE_GUI
	(void)names;  // How to display names next to planets and stars (bodies)?

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "N-Body Simulation");
	SetTargetFPS(60);
	DisableCursor();

	// FROM: https://www.raylib.com/examples/models/loader.html?name=models_skybox
	// Load skybox model
	Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
	Model skybox = LoadModelFromMesh(cube);

	// Load skybox shader and set required locations
	// NOTE: Some locations are automatically set at shader loading
	skybox.materials[0].shader = LoadShader(TextFormat("resources/shaders/glsl%i/skybox.vs", GLSL_VERSION),
											TextFormat("resources/shaders/glsl%i/skybox.fs", GLSL_VERSION));

	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]) { 0 }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

	// Load cubemap shader and setup required shader locations
	Shader shdrCubemap = LoadShader(TextFormat("resources/shaders/glsl%i/cubemap.vs", GLSL_VERSION),
									TextFormat("resources/shaders/glsl%i/cubemap.fs", GLSL_VERSION));

	SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

	Image img = LoadImage("resources/skybox.png");
	skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA
	UnloadImage(img);

	Vector3 prev_sun_pos = {
		(float)bodies[0].pos[0] / POS_RENDER_SCALE,
		(float)bodies[0].pos[1] / POS_RENDER_SCALE,
		(float)bodies[0].pos[2] / POS_RENDER_SCALE
	};

	Camera3D camera = {
		.position = Vector3Add(prev_sun_pos, (Vector3){0.0f, 4.0f, 10.0f}),
		.target = prev_sun_pos,
		.up = { 0.0f, 1.0f, 0.0f },
		.fovy = 45.0f,
		.projection = CAMERA_PERSPECTIVE
	};

	float time_scale = 1.0f;  // 1.0 = real time; < 1.0 = slow; > 1.0 = fast
	const double tick_interval = 1.0 / TICK_SPEED;
	double tick_accumulator = 0.0;

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_UP)) time_scale *= 2.0f;
		if (IsKeyPressed(KEY_DOWN)) time_scale *= 0.5f;

		// Clamp to reasonable range
		if (time_scale < 0.01f) time_scale = 0.01f;
		if (time_scale > 100.0f) time_scale = 100.0f;

		UpdateCamera(&camera, CAMERA_FREE);

		double frame_time = GetFrameTime();
		tick_accumulator += frame_time;

		while (tick_accumulator >= tick_interval) {
			// Only update if enough time has passed (n physics ticks per second)
			//update_bodies(bodies, n_bodies, tick_interval * time_scale);
			tick_accumulator -= tick_interval;
		}

		// if (1 == 1) {
		Vector3 current_sun_pos = {
			(float)bodies[0].pos[0] / POS_RENDER_SCALE,
			(float)bodies[0].pos[1] / POS_RENDER_SCALE,
			(float)bodies[0].pos[2] / POS_RENDER_SCALE
		};

		//Vector3 sun_delta = Vector3Subtract(current_sun_pos, prev_sun_pos);

		//camera.position = Vector3Add(camera.position, sun_delta);
		//camera.target = Vector3Add(camera.target, sun_delta);

		prev_sun_pos = current_sun_pos;  // We do not want to update this if its disabled
		//}
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

		(void)center_x;
		(void)center_z;
		(void)camera_height;

                //camera.up = (Vector3){ 0.0f, 0.0f, -1.0f }; // North is -Z

		//camera.position = (Vector3){ (float)center_x / POS_RENDER_SCALE, camera_height / POS_RENDER_SCALE, (float)center_z / POS_RENDER_SCALE };
                //camera.target = (Vector3){ (float)center_x, 0.0f, (float)center_z };

                // printf("POS %f %f %f\n", camera.position.x, camera.position.y, camera.position.z);
                // printf("Loop %f %f %f\n", (float)bodies[0].pos[0], (float)bodies[0].pos[1], (float)bodies[0].pos[2]);
		printf("%f\n", POS_RENDER_SCALE);
                // printf("TI: %f\n", tick_interval);

                // Drawing
                BeginDrawing();
                ClearBackground(BLANK);

                BeginMode3D(camera);

		// Skybox disable depth testing so it always appears in background
		rlDisableBackfaceCulling();
		rlDisableDepthMask();

		DrawModel(skybox, camera.position, 1.0f, WHITE);

		//rlEnableBackfaceCulling();
		rlEnableDepthMask();

		DrawGrid(10, 1.0f);

		VectorT sun_pos[3] = {bodies[0].pos[0], bodies[0].pos[1], bodies[0].pos[2]};

		for (size_t i = 0; i < n_bodies; i++) {
			bodies[i].pos[0] = bodies[i].pos[0] - sun_pos[0];
			bodies[i].pos[1] = bodies[i].pos[1] - sun_pos[1];
			bodies[i].pos[2] = bodies[i].pos[2] - sun_pos[2];
                        Vector3 pos = {
                                (float)bodies[i].pos[0] / POS_RENDER_SCALE,
                                (float)bodies[i].pos[1] / POS_RENDER_SCALE,
                                (float)bodies[i].pos[2] / POS_RENDER_SCALE
                        };
                        float radius = (float)(bodies[i].radius * 1e-9) * PLANET_RENDER_SCALE;
			if (radius > MAX_PLANET_RENDER_SCALE) radius = MAX_PLANET_RENDER_SCALE;
			else if (radius < MIN_PLANET_RENDER_SCALE) radius = MIN_PLANET_RENDER_SCALE;
			if (i == 0) printf("R: %f\n", radius);
                        DrawSphere(pos, radius, (Color){ colors[i][0], colors[i][1], colors[i][2], 255 });
		}

		// Collect label draw info for a second 2D pass
		typedef struct { Vector2 sp; int bodyIndex; } LabelInfo;
		LabelInfo labelBuf[128];
		int labelCount = 0;

		for (size_t i = 0; i < n_bodies; i++) {
			Vector3 world_pos = {
				(float)bodies[i].pos[0],
				(float)bodies[i].pos[1],
				(float)bodies[i].pos[2]
			};

			Vector3 sun_offset = { sun_pos[0], sun_pos[1], sun_pos[2] };
			Vector3 rel_pos    = Vector3Subtract(world_pos, sun_offset);
			Vector3 scaled_pos = Vector3Scale(rel_pos, 1.0f / POS_RENDER_SCALE);

			// 3D line from Sun to planets
			//if (i != 0) {
			//	DrawLine3D((Vector3){0,0,0}, scaled_pos, (Color){180,180,180,255});
			//}

			Vector3 cam_forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));

			// Only label if planet is in front of camera
			Vector3 to_obj = Vector3Normalize(Vector3Subtract(scaled_pos, camera.position));
			float dp = Vector3DotProduct(cam_forward, to_obj);
			if (dp > 0.0f) {
				Vector2 sp = GetWorldToScreen(scaled_pos, camera);
				if (labelCount < (int)(sizeof(labelBuf)/sizeof(labelBuf[0]))) {
					labelBuf[labelCount++] = (LabelInfo){ sp, (int)i };
				}
			}
			}

                EndMode3D();

                DrawText(TextFormat("Tick speed: %.2f", (float)TICK_SPEED), 10, 10, 20, RAYWHITE);
				DrawText(TextFormat("Time Scale: %.2fx", time_scale), 10, 30, 20, RAYWHITE);

				for (int k = 0; k < labelCount; k++) {
					int i = labelBuf[k].bodyIndex;

					// Midpoint in space for nicer label anchoring
					Vector3 world_sun = { (float)bodies[0].pos[0], (float)bodies[0].pos[1], (float)bodies[0].pos[2] };
					Vector3 world_pl  = { (float)bodies[i].pos[0], (float)bodies[i].pos[1], (float)bodies[i].pos[2] };
					Vector3 world_mid = Vector3Scale(Vector3Add(world_sun, world_pl), 0.5f);

					// Convert midpoint to render space :: apply sun offset + POS_RENDER_SCALE
					Vector3 mid_rel   = Vector3Subtract(world_mid, (Vector3){ sun_pos[0], sun_pos[1], sun_pos[2] });
					Vector3 mid_scaled= Vector3Scale(mid_rel, 1.0f / POS_RENDER_SCALE);
					Vector2 sp        = GetWorldToScreen(mid_scaled, camera);

					// Physical distance :: never apply POS_RENDER_SCALE here!!
					double dx = (double)bodies[i].pos[0] - (double)bodies[0].pos[0];
					double dy = (double)bodies[i].pos[1] - (double)bodies[0].pos[1];
					double dz = (double)bodies[i].pos[2] - (double)bodies[0].pos[2];
					double dist_sim = sqrt(dx*dx + dy*dy + dz*dz);

					// Convert to AU for display
					double to_AU   = space_conversion_table[SPACE_TYPE][SPACE_TYPE_AU];
					double dist_AU = dist_sim * to_AU;

					const char* label = TextFormat("%s  %.3f AU", names[i], dist_AU);
					int fontSize = 18;
					int tw = MeasureText(label, fontSize);

					int x = (int)sp.x + 8;
					int y = (int)sp.y - (fontSize + 2);

					// simple background
					DrawRectangle(x-4, y-2, tw+8, fontSize+6, (Color){0,0,0,140});
					DrawText(label, x, y, fontSize, RAYWHITE);
				}

                EndDrawing();
	}

	UnloadShader(skybox.materials[0].shader);
	UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);

	UnloadModel(skybox);        // Unload skybox model

	EnableCursor();

	CloseWindow();
	return 0;
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


