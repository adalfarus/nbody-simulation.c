#include "n_body.h"

#include <stdio.h>

#ifdef USE_GUI
#include "raylib.h"
#endif

void update_bodies(BodySnapshot bodies[], double tick_speed) {
	(void)bodies;
	(void)tick_speed;
#ifdef USE_NEWTON_SIMULATION
	(void)bodies;
	(void)tick_speed;
#endif
}

int start_simulation(BodySnapshot bodies[]) {
	printf("%d\n", USE_GUI);
#ifdef USE_GUI
	InitWindow(800, 600, "N-Body Simulation");
	SetTargetFPS(60);

	Camera3D camera = {
		.position = { 0.0f, 20.0f, 40.0f },
		.target = { 0.0f, 0.0f, 0.0f },
		.up = { 0.0f, 1.0f, 0.0f },
		.fovy = 45.0f,
		.projection = CAMERA_PERSPECTIVE
	};
	
	double tick_speed = TICK_SPEED;

	while (!WindowShouldClose()) {
		// Adjust tick speed
		if (IsKeyPressed(KEY_UP)) tick_speed *= 2.0;
		if (IsKeyPressed(KEY_DOWN)) tick_speed *= 0.5;
		if (tick_speed < LOW_TICK_SPEED) tick_speed = LOW_TICK_SPEED;

		// Update
		update_bodies(bodies, tick_speed);

		// Drawing
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode3D(camera);

		for (int i = 0; i < N_BODIES; i++) {
			Vector3 pos = {
				(float)bodies[i].pos[0],
				(float)bodies[i].pos[1],
				(float)bodies[i].pos[2]
			};
			float radius = (float)(bodies[i].radius * 1e-9);
			DrawSphere(pos, radius, WHITE);
		}

		EndMode3D();

		DrawText(TextFormat("Tick speed: %.2e", tick_speed), 10, 10, 20, RAYWHITE);

		EndDrawing();
	}
	
	CloseWindow();

#else
	printf("Headless mode is not yet supported!\n");
	(void)bodies;
#endif
	return 0;
}


