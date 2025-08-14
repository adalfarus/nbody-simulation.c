#include "nbody.h"

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>
#include <string.h> // For memset
#include <signal.h>
#include <stdlib.h>  // For malloc

double G_CONST_SIM;

#ifdef INCLUDE_NEWTON_SIM
bool sim_newton_init(SimulationState* S) {
	(void)S;
	return true;
}
bool sim_newton_step(SimulationState* S, BodySnapshot *bodies, size_t nbodies, double dt_sim) {
	for (size_t i = 0; i < nbodies; ++i) {
		S->accelerations[i][0] = (VecT)0;
		S->accelerations[i][1] = (VecT)0;
		S->accelerations[i][2] = (VecT)0;
	}

	const VecT eps2 = (VecT)1e-20;    // avoid division error
	for (size_t i = 0; i < nbodies; ++i) {
		const VecT xi = (VecT)bodies[i].pos[0];
		const VecT yi = (VecT)bodies[i].pos[1];
		const VecT zi = (VecT)bodies[i].pos[2];

		for (size_t j = 0; j < nbodies; ++j) {
			if (i == j) continue;

			const VecT dx = (VecT)bodies[j].pos[0] - xi;
			const VecT dy = (VecT)bodies[j].pos[1] - yi;
			const VecT dz = (VecT)bodies[j].pos[2] - zi;

			const VecT r2 = dx*dx + dy*dy + dz*dz;
			if (r2 <= eps2) continue;

			const VecT r   = VEC_SQRT(r2);
			const VecT inv_r3 = (VecT)1.0 / (r2 * r);

			// s = G * m_j / r^3
			const VecT s = (VecT)(G_CONST_SIM * (double)bodies[j].mass) * inv_r3;

			S->accelerations[i][0] += s * dx;
			S->accelerations[i][1] += s * dy;
			S->accelerations[i][2] += s * dz;
		}
	}
	const VecT dt = (VecT)dt_sim;

	for (size_t i = 0; i < nbodies; ++i) {
		bodies[i].vel[0] += S->accelerations[i][0] * dt;
		bodies[i].vel[1] += S->accelerations[i][1] * dt;
		bodies[i].vel[2] += S->accelerations[i][2] * dt;

		bodies[i].pos[0] += (PosT)(bodies[i].vel[0] * dt);
		bodies[i].pos[1] += (PosT)(bodies[i].vel[1] * dt);
		bodies[i].pos[2] += (PosT)(bodies[i].vel[2] * dt);
	}
	return true;
}
void sim_newton_exit(SimulationState* S) {
	(void)S;
}
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

#define VELOCITY_VECTOR_SCALE 1.0

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_UP)) time_scale *= 2.0f;
		if (IsKeyPressed(KEY_DOWN)) time_scale *= 0.5f;

		// Clamp to reasonable range
		if (time_scale < 0.01f) time_scale = 0.01f;
		if (time_scale > 100000000.0f) time_scale = 100000000.0f;

		// UpdateCamera(&camera, CAMERA_FREE);

		float dt = GetFrameTime();

		// mouse-look
		Vector2 md = GetMouseDelta();
		float yaw   = -md.x * 0.003f;   // tweak sensitivity
		float pitch = -md.y * 0.003f;

		Vector3 f = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
		Matrix  yawM   = MatrixRotateY(yaw);
		Vector3 fYaw   = Vector3Transform(f, yawM);
		Vector3 right  = Vector3Normalize(Vector3CrossProduct(fYaw, camera.up));
		Matrix  pitchM = MatrixRotate(right, pitch);

		// clamp pitch a bit to avoid flip (optional)
		Vector3 fNew = Vector3Transform(fYaw, pitchM);
		camera.target = Vector3Add(camera.position, fNew);

		// movement (WASDQE)
		float baseSpeed = 25.0f;                  // render units per second
		float boost     = IsKeyDown(KEY_LEFT_SHIFT) ? 6.0f : 1.0f;
		float speed     = baseSpeed * boost;

		Vector3 move = {0};
		if (IsKeyDown(KEY_W)) move = Vector3Add(move, Vector3Scale(fNew,   speed*dt));
		if (IsKeyDown(KEY_S)) move = Vector3Add(move, Vector3Scale(fNew,  -speed*dt));
		if (IsKeyDown(KEY_D)) move = Vector3Add(move, Vector3Scale(right,  speed*dt));
		if (IsKeyDown(KEY_A)) move = Vector3Add(move, Vector3Scale(right, -speed*dt));
		if (IsKeyDown(KEY_E)) move = Vector3Add(move, Vector3Scale(camera.up,  speed*dt));
		if (IsKeyDown(KEY_Q)) move = Vector3Add(move, Vector3Scale(camera.up, -speed*dt));

		// mouse wheel zoom (dolly along forward)
		float zoomSpeed = 50.0f;  // render units per wheel step
		float wheel = GetMouseWheelMove();
		if (wheel != 0.0f) {
			move = Vector3Add(move, Vector3Scale(fNew, wheel * zoomSpeed * dt));
		}

		camera.position = Vector3Add(camera.position, move);
		camera.target   = Vector3Add(camera.target,   move);

		double frame_time = GetFrameTime();
		tick_accumulator += frame_time * time_scale;

		while (tick_accumulator >= tick_interval) {
			// Only update if enough time has passed (n physics ticks per second)
			update_bodies(bodies, n_bodies, tick_interval);
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

                // === Add bufer based on largest radius ===
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

		VecT sun_pos[3] = {bodies[0].pos[0], bodies[0].pos[1], bodies[0].pos[2]};

		const float invScale = 1.0f / (float)POS_RENDER_SCALE; // World -> render scale once
		Vector3 cam_forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));

		// Collect label draw info for a second 2D pass
		typedef struct {
			int x, y;
			int fs, tw;
			Color text_color, bg_color;
			char text[96];   // owned storage for label text
		} LabelInfo;
		LabelInfo labelBuf[128];
		int labelCount = 0;

		for (size_t i = 0; i < n_bodies; i++) {
			bodies[i].pos[0] = bodies[i].pos[0] - sun_pos[0];
			bodies[i].pos[1] = bodies[i].pos[1] - sun_pos[1];
			bodies[i].pos[2] = bodies[i].pos[2] - sun_pos[2];
			Vector3 pos = {
				(float)bodies[i].pos[0] * invScale,
				(float)bodies[i].pos[1] * invScale,
				(float)bodies[i].pos[2] * invScale
			};
			float radius = (float)bodies[i].radius * PLANET_RENDER_SCALE;
			//if (radius > MAX_PLANET_RENDER_SCALE) radius = MAX_PLANET_RENDER_SCALE;
			//else if (radius < MIN_PLANET_RENDER_SCALE) radius = MIN_PLANET_RENDER_SCALE;
			//if (i == 0) printf("R: %f\n", radius);
			radius = Clamp(radius, MIN_PLANET_RENDER_SCALE, MAX_PLANET_RENDER_SCALE);
			Color c = (Color){ colors[i][0], colors[i][1], colors[i][2], 255 };
			DrawSphere(pos, radius, c);

			// Velocity Vectors
			//Vector3 vSim = { (float)bodies[i].vel[0], (float)bodies[i].vel[1], (float)bodies[i].vel[2] };

			double to_sec = time_conversion_table[TIME_TYPE][TIME_TYPE_SECOND];
			Vector3 vSim = {
				(float)(bodies[i].vel[0] / to_sec),
				(float)(bodies[i].vel[1] / to_sec),
				(float)(bodies[i].vel[2] / to_sec)
			};

			float invScale = 1.0f / (float)POS_RENDER_SCALE;
			Vector3 v_render_s = Vector3Scale(vSim, invScale);

			//Vector3 vRender = Vector3Scale(vAU, AU_TO_RENDER * VELOCITY_VECTOR_SCALE);
			//Vector3 vRender = Vector3Scale(vSim, invScale * VELOCITY_VECTOR_SCALE);
			//Vector3 end = Vector3Add(pos, vRender);

			Vector3 vRender = Vector3Scale(v_render_s, time_scale * VELOCITY_VECTOR_SCALE);  //
			Vector3 end = Vector3Add(pos, vRender);

			DrawLine3D(pos, end, c);

			Vector3 dir  = Vector3Normalize(vRender);
			if (Vector3LengthSqr(dir) > 1e-12f) {
				Vector3 side = Vector3Normalize(Vector3CrossProduct(dir, camera.up));
				float ah = 0.15f * Vector3Length(vRender);           // arrowhead size 15% opf the arrow
				Vector3 tipL = Vector3Add(end, Vector3Scale(Vector3Add(Vector3Scale(dir,-1), side), ah));
				Vector3 tipR = Vector3Add(end, Vector3Scale(Vector3Add(Vector3Scale(dir,-1), Vector3Scale(side,-1)), ah));
				DrawLine3D(end, tipL, c);
				DrawLine3D(end, tipR, c);
			}

			Vector3 to_obj = Vector3Normalize(Vector3Subtract(pos, camera.position));
			float dp = Vector3DotProduct(cam_forward, to_obj);
			if (dp > 0.0f) {
				// Speed magnitude in *km/s* (convert sim units -> km, time -> s)
				double to_km = space_conversion_table[SPACE_TYPE][SPACE_TYPE_KM];
				double to_s  = time_conversion_table[TIME_TYPE][TIME_TYPE_SECOND];
				double vmag  = sqrt((double)bodies[i].vel[0]*bodies[i].vel[0] +
				(double)bodies[i].vel[1]*bodies[i].vel[1] +
				(double)bodies[i].vel[2]*bodies[i].vel[2]);
				double speed_km_s = vmag * (to_km / to_s);

				// Cardinal from horizontal heading (assume +Z = North, +X = East)
				float ang = atan2f(vSim.x, vSim.z);  // radians, [-pi,pi], 0 -> North
				static const char* DIR8[8] = {"N","NE","E","SE","S","SW","W","NW"};
				int idx = (int)floorf((ang + PI) / (PI/4.0f) + 0.5f) & 7;

				// Label near arrow tip
				Vector2 sp = GetWorldToScreen(end, camera);
				// const char* txt = TextFormat("%s  %.2f km/s", DIR8[idx], speed_km_s);

				int fs = 16;
				int x = (int)sp.x + 8;
				int y = (int)sp.y - (fs + 2);

				if (labelCount < (int)(sizeof(labelBuf)/sizeof(labelBuf[0]))) {
					LabelInfo li = {
						.x = x,
						.y = y,
						.fs = fs,
						.text_color = RAYWHITE,
						.bg_color = (Color){0,0,0,140}
					};
					snprintf(li.text, sizeof(li.text), "%s  %.2f km/s", DIR8[idx], speed_km_s);
					li.tw = MeasureText(li.text, li.fs);
					//labelBuf[labelCount++] = li;
				}
			}
		}

		for (size_t i = 0; i < n_bodies; i++) {
			Vector3 world_pos = {
				(float)bodies[i].pos[0],
				(float)bodies[i].pos[1],
				(float)bodies[i].pos[2]
			};

			//Vector3 sun_offset = { sun_pos[0], sun_pos[1], sun_pos[2] };
			//Vector3 rel_pos    = Vector3Subtract(world_pos, sun_offset);
			Vector3 scaled_pos = Vector3Scale(world_pos, 1.0f / POS_RENDER_SCALE);  //rel_pos

			// 3D line from Sun to planets
			//if (i != 0) {
			//	DrawLine3D((Vector3){0,0,0}, scaled_pos, (Color){180,180,180,255});
			//}

			Vector3 cam_ws = {
				camera.position.x * POS_RENDER_SCALE,
				camera.position.y * POS_RENDER_SCALE,
				camera.position.z * POS_RENDER_SCALE
			};

			printf("%f %f %f\n", camera.position.x, camera.position.y, camera.position.z);

			// Only label if planet is in front of camera
			Vector3 to_obj = Vector3Normalize(Vector3Subtract(scaled_pos, camera.position));
			float dp = Vector3DotProduct(cam_forward, to_obj);
			if (dp > 0.0f) {
				// Midpoint in space for nicer label anchoring
				//Vector3 world_sun = { (float)bodies[0].pos[0], (float)bodies[0].pos[1], (float)bodies[0].pos[2] };
				Vector3 world_pl  = { (float)bodies[i].pos[0], (float)bodies[i].pos[1], (float)bodies[i].pos[2] };
				Vector3 scaled_pl = Vector3Scale(world_pl, 1.0f / POS_RENDER_SCALE);

				//Vector3 world_mid = Vector3Scale(Vector3Add(camera.position, world_pl), 0.5f);  // world_sun

				// Convert midpoint to render space :: apply sun offset + POS_RENDER_SCALE
				//Vector3 mid_rel   = Vector3Subtract(world_mid, camera.position);  // (Vector3){ sun_pos[0], sun_pos[1], sun_pos[2] }
				//Vector3 mid_scaled= Vector3Scale(mid_rel, 1.0f / POS_RENDER_SCALE);
				Vector2 sp        = GetWorldToScreen(scaled_pl, camera);  // mid_scaled

				// Physical distance :: never apply POS_RENDER_SCALE here!!
				double dx = (double)bodies[i].pos[0] - (double)cam_ws.x;
				double dy = (double)bodies[i].pos[1] - (double)cam_ws.y;
				double dz = (double)bodies[i].pos[2] - (double)cam_ws.z;
				double dist_sim = sqrt(dx*dx + dy*dy + dz*dz);

				// Convert to AU for display
				double to_AU   = space_conversion_table[SPACE_TYPE][SPACE_TYPE_AU];
				double dist_AU = dist_sim * to_AU;

				// const char* label = TextFormat("%s  %.3f AU", names[i], dist_AU);
				int fontSize = 18;

				int x = (int)sp.x + 8;
				int y = (int)sp.y - (fontSize + 2);

				if (labelCount < (int)(sizeof(labelBuf)/sizeof(labelBuf[0]))) {
					LabelInfo li2 = {
						.x = x,
						.y = y,
						.fs = 18,
						.text_color = RAYWHITE,
						.bg_color = (Color){0,0,0,140}
					};
					snprintf(li2.text, sizeof(li2.text), "%s  %.3f AU", names[i], dist_AU);
					li2.tw = MeasureText(li2.text, li2.fs);
					labelBuf[labelCount++] = li2;
				}
			}
		}

		EndMode3D();

		DrawText(TextFormat("Tick speed: %.2f", (float)TICK_SPEED), 10, 10, 20, RAYWHITE);
		DrawText(TextFormat("Time Scale: %.2fx", time_scale), 10, 30, 20, RAYWHITE);
		DrawText(TextFormat("Body Scale: %.9f", PLANET_RENDER_SCALE), 10, 50, 20, RAYWHITE);
		DrawText(TextFormat("Pos Scale: %.2f", POS_RENDER_SCALE), 10, 70, 20, RAYWHITE);

		for (int k = 0; k < labelCount; k++) {
			DrawRectangle(labelBuf[k].x-4, labelBuf[k].y-2, labelBuf[k].tw+8, labelBuf[k].fs+6, labelBuf[k].bg_color);
			DrawText(labelBuf[k].text, labelBuf[k].x, labelBuf[k].y, labelBuf[k].fs, labelBuf[k].text_color);
		}

		// Compass
		Vector2 cc = { GetScreenWidth()*0.5f, 26.0f };
		DrawLine(cc.x-30, cc.y, cc.x+30, cc.y, GRAY);
		DrawLine(cc.x, cc.y-30, cc.x, cc.y+30, GRAY);
		DrawText("N", (int)cc.x-5, (int)(cc.y-30-14), 14, GRAY);
		DrawText("S", (int)cc.x-4, (int)(cc.y+30+2), 14, GRAY);
		DrawText("W", (int)(cc.x-30-10), (int)cc.y-7, 14, GRAY);
		DrawText("E", (int)(cc.x+30+4), (int)cc.y-7, 14, GRAY);

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



void print_bodies(BodySnapshot bodies[], size_t n_bodies) {
	for (size_t i = 0; i < n_bodies; i++) {
		BodySnapshot body = bodies[i];
		printf("Body %zu\nMass: %" PRIReal "\nRadius: %" PRIReal "\nPos: %" PRIPosT "/%" PRIPosT "/%" PRIPosT "\nVel: %" PRIVecT "/%" PRIVecT "/%" PRIVecT "\n", i, body.mass, body.radius, body.pos[0], body.pos[1], body.pos[2], body.vel[0], body.vel[1], body.vel[2]);
	}
}
static inline double compute_sim_G(void) {
	const double G_SI = 6.67430e-11; // m^3 kg^-1 s^-2

	double L = space_conversion_table[SPACE_TYPE_M][SPACE_TYPE];        // m to SIM_LENGTH
	double M = mass_conversion_table[MASS_TYPE_KG][MASS_TYPE];          // kg to SIM_MASS
	double T = time_conversion_table[TIME_TYPE_SECOND][TIME_TYPE];      // s to SIM_TIME

	return G_SI * (L*L*L) / (M * T*T);
}
static double monotonic_time_s(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

typedef bool (*ShouldExitFn)(void);
typedef bool (*InitFn)(SimulationState *S);
typedef bool (*NeedUpdateFn)(SimulationState *S);
typedef void (*StepFn)(SimulationState *S, BodySnapshot *bodies, size_t nbodies, bool did_update);  // bool means either okay everything can go or its getting too slow
typedef int (*ExitFn)(SimulationState *S);

bool guiShouldExit(void);
bool guiInit(SimulationState *S);
bool guiNeedUpdate(SimulationState *S);
void guiStep(SimulationState *S, BodySnapshot *bodies, size_t nbodies, bool did_update);
int guiExit(SimulationState *S);


static volatile sig_atomic_t term_exit_requested = 0;
static void handle_sigint(int sig) {
	(void)sig;
	term_exit_requested = 1;
}

bool termShouldExit(void) {
	return term_exit_requested;
}

bool termInit(SimulationState *S) {
	(void)S;
	signal(SIGINT, handle_sigint);
	return true;
}

bool termNeedUpdate(SimulationState *S) {
	(void)S;
	static double prev_real = 0.0;
	static bool initialized = false;

	double now_real = monotonic_time_s();  // e.g., returns time in seconds (double)

	if (!initialized) {
		prev_real = now_real;
		initialized = true;
		return false;
	}

	double dt_real = now_real - prev_real;

	if (dt_real >= 1.0) {
		prev_real = now_real;
		return true;
	}

	return false;
}

void termStep(SimulationState *S, BodySnapshot *bodies, size_t nbodies, bool did_update) {
        if (!did_update) return;
	printf("t_sim=%zu, nbodies=%zu, loss=%lf%%\n", S->t_sim, nbodies, S->tick_accumulator / 1.0 / S->config.base_physics_dt_sim * S->config.time_scale);
	print_bodies(bodies, nbodies);
	printf("\n");
}

int termExit(SimulationState *S) {
	(void)S;
	term_exit_requested = 0;
	return 0;
}

int start_simulation(const Body *bodies, size_t nbodies, size_t floating_origin_idx, SimulationConfig* cfg) {
#ifdef SUPPORT_EXTREME_BODIES  // The gravitational constant only affects very very large or small bodies
	double G_CONST_SIM = compute_sim_G();  // Convert G_CONST
#else
	double G_CONST_SIM = 1.0;
#endif
	(void)G_CONST_SIM;

	const char* names[nbodies];
	uint8_t colors[nbodies][3];
	BodySnapshot starting_bodies[nbodies];
	VecT accelerations[nbodies][3];

	for (size_t i = 0; i < nbodies; i++) {
		names[i] = bodies[i].name;
		
		colors[i][0] = bodies[i].color[0];
		colors[i][1] = bodies[i].color[1];
		colors[i][2] = bodies[i].color[2];

		double conversion_mass = mass_conversion_table[bodies[i].mass_type][MASS_TYPE];
		double conversion_space = space_conversion_table[bodies[i].space_type][SPACE_TYPE];
		double conversion_time = time_conversion_table[bodies[i].time_type][TIME_TYPE];

		starting_bodies[i] = (BodySnapshot){
			.mass = (Real)(bodies[i].mass * conversion_mass),
			.radius = (Real)(bodies[i].radius * conversion_space),
			.pos = {
				(VecT)(bodies[i].pos[0] * conversion_space),
				(VecT)(bodies[i].pos[1] * conversion_space),
				(VecT)(bodies[i].pos[2] * conversion_space)
			},
			.vel = {
				(VecT)(bodies[i].vel[0] * conversion_space / conversion_time),
				(VecT)(bodies[i].vel[1] * conversion_space / conversion_time),
				(VecT)(bodies[i].vel[2] * conversion_space / conversion_time)
			},
			// .acc = { 0, 0, 0 }
		};
	}

	print_bodies(starting_bodies, nbodies);

	(void)cfg;  // Because copying is apparently "not using"
	SimulationState S = (SimulationState) {
		.t_sim = 0,
		.tick_accumulator = 0.0,
		.paused = false,
		.body_snapshots_idx = 0,

		.simulation_step_fn_idx = 0,
		.simulation_step_fn_count = 0,
		.simulation_step_fns = NULL,

		.nbodies = nbodies,
		.names = names,
		.colors = colors,
		.accelerations = accelerations,

		.config = *cfg
	};

	if (S.config.keep_history && S.config.history_len > 0) {
		S.body_snapshots.capacity = S.config.history_len;
		S.body_snapshots.head = 0;
		S.body_snapshots.filled = false;
		S.body_snapshots.frames = calloc(S.config.history_len, sizeof(SnapshotFrame));

		for (size_t i = 0; i < S.config.history_len; i++) {
			S.body_snapshots.frames[i].count = nbodies;
			S.body_snapshots.frames[i].buf = malloc(nbodies * sizeof(BodySnapshot));
		}
	} else {
		S.body_snapshots.capacity = 0;
		S.body_snapshots.frames = NULL;
	}

	history_push(&S.body_snapshots, starting_bodies, S.nbodies);

	for (size_t i = 0; i < S.config.simulations_count; i++) {
		if (S.config.simulations[i].enabled && S.config.simulations[i].init) {
			S.config.simulations[i].init(&S);  // Init simulation type
		}
	}

	S.simulation_step_fn_count = 0;
	if (S.config.simulation_order && S.config.simulation_order_count > 0) {
		S.simulation_step_fns = malloc(S.config.simulation_order_count * sizeof *S.simulation_step_fns);
		if (!S.simulation_step_fns) { perror("malloc"); return 1; }

		for (size_t i = 0; i < S.config.simulation_order_count; i++) {
			size_t simulation_idx = S.config.simulation_order[i];
			
			if (simulation_idx < S.config.simulations_count) {
				SimulationType *sim = &S.config.simulations[simulation_idx];
				if (sim->enabled && sim->step) {
					S.simulation_step_fns[S.simulation_step_fn_count++] = sim->step;
				}
			}
			S.simulation_step_fn_count = i+1;
		}
	} else {  // Just sims in the order of the list
		S.simulation_step_fns = malloc(S.config.simulations_count *
		sizeof *S.simulation_step_fns);
		if (!S.simulation_step_fns) { perror("malloc"); return 1; }

		for (size_t i = 0; i < S.config.simulations_count; i++) {
			SimulationType *sim = &S.config.simulations[i];
			if (sim->enabled && sim->step) {
				S.simulation_step_fns[S.simulation_step_fn_count++] = sim->step;
			}
		}
	}

	if (S.simulation_step_fn_count == 0) {
		fprintf(stderr, "No enabled simulation step functions!\n");
		return 1;
	}

	ShouldExitFn shouldExit = NULL;
	InitFn uiInit = NULL;
	NeedUpdateFn uiNeedUpdate = NULL;
	StepFn uiStep = NULL;
	ExitFn uiExit = NULL;

	if (S.config.display.enabled) {
#ifdef ENABLE_GUI
		shouldExit = guiShouldExit;
		uiNeedUpdate = guiNeedUpdate;
		uiInit = guiInit;
		uiStep = guiStep;
		uiExit = guiExit;
#else
		printf("To use the gui you need to enable it during compilation using #define ENABLE_GUI");
		return 1;
#endif
	} else {
		shouldExit = termShouldExit;
		uiNeedUpdate = termNeedUpdate;
		uiInit = termInit;
		uiStep = termStep;
		uiExit = termExit;
	}

	if (!uiInit(&S)) return 1;

	double prev_real = monotonic_time_s();
	const double tick_interval = 1.0 / S.config.base_physics_dt_sim;
	bool did_update;

	(void)floating_origin_idx;
	while (!shouldExit()) {  //// FLOATING ORIGIN
		did_update = false;
		double now_real = monotonic_time_s();
		double dt_real = now_real - prev_real;
		prev_real = now_real;

		// clamp big pauses (alt-tab, breakpoint)
		if (dt_real > 0.5) dt_real = 0.5;
		if (dt_real < 0.0) dt_real = 0.0;

		if (S.config.time_scale < S.config.time_scale_min) S.config.time_scale = S.config.time_scale_min;
		if (S.config.time_scale > S.config.time_scale_max) S.config.time_scale = S.config.time_scale_max;
		double dt_sim = dt_real * S.config.time_scale;

		S.tick_accumulator += dt_sim;
		if (S.tick_accumulator > S.config.max_accumulator_backlog) S.tick_accumulator = S.config.max_accumulator_backlog;


		while (!uiNeedUpdate(&S) && S.tick_accumulator >= tick_interval) {  // Only update if enough time has passed (n physics ticks per second)
			// Copy last frame one forward:
			BodySnapshot *next = history_clone_for_write(&S.body_snapshots, S.nbodies);  // Maybe just give pointer to current vals and where to write?

			SimStepFn step = S.simulation_step_fns[S.simulation_step_fn_idx];
			step(&S, next, S.nbodies, tick_interval * S.config.physics_tick_step_size_multiplier);
			
			history_commit(&S.body_snapshots);
			
			S.tick_accumulator -= tick_interval;
			S.simulation_step_fn_idx = (S.simulation_step_fn_idx + 1) % S.simulation_step_fn_count;
			S.t_sim += 1;
			did_update = true;
		}

		size_t last = history_last_index(&S.body_snapshots);
		BodySnapshot *current_frame = S.body_snapshots.frames
		? S.body_snapshots.frames[last].buf
		: starting_bodies; // fallback if no history
		uiStep(&S, current_frame, S.nbodies, did_update);  // Maybe just give snapshot frame?
	}

	for (size_t i = 0; i < S.config.simulations_count; i++) {
		if (S.config.simulations[i].enabled && S.config.simulations[i].exit) {
			S.config.simulations[i].exit(&S);  // Init simulation type
		}
	}

	if (S.body_snapshots.frames) {
		for (size_t i = 0; i < S.body_snapshots.capacity; ++i) free(S.body_snapshots.frames[i].buf);
		free(S.body_snapshots.frames);
	}
	free(S.simulation_step_fns);

	return uiExit(&S);
}
