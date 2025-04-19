#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "interfaces.h"
#include "draw.h"

void handleCameraEvents(Camera2D* camera)
{
	// Translate based on mouse left click
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		// Prevent panning outside of area
		if (camera->zoom > 1.0f)
		{
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f / camera->zoom);
			camera->target = Vector2Add(camera->target, delta);
		}
	}
	// Zoom based on mouse right click
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		// Get the world point that is under the mouse
		Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);
		camera->offset = GetMousePosition();
		camera->target = mouseWorldPos;
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		// Uses log scaling to provide consistent zoom speed
		float deltaX = GetMouseDelta().x;
		if ((deltaX > 0.0f) || (camera->zoom > 1.0f)) // prevent zooming outside
		{
			float scale = 0.005f * deltaX;
			camera->zoom = Clamp(expf(logf(camera->zoom) + scale), 0.125f, 64.0f);
		}
	}
	if (IsKeyPressed(KEY_R))
	{
		// Reset view, zoom and panning
		camera->zoom = 1.0f;
		camera->target = (Vector2){ 0.0f, 0.0f };
		camera->offset = (Vector2){ 0.0f, 0.0f };
	}
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BEV Viewer");
	SetTargetFPS(60);
	Camera2D camera = { 0 };
	camera.zoom = 1.0f;

	/* Initialization */
	State state;
	DetectionList detList;
	memset(&detList, 0, sizeof(DetectionList));
	detList.detections[0].posX = 40;
	detList.detections[0].posY = 20;
	detList.detections[1].posX = 20;
	detList.detections[1].posY = 25;
	detList.numValidDets = 2;
	state.pDetList = &detList;
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		/* Event handling */
		handleCameraEvents(&camera);

		/* Update state */

		/* Drawing */
		BeginDrawing();

			ClearBackground(BG_COLOR);

			BeginMode2D(camera);

				drawAxis();
		
				drawDetections(state.pDetList);

			EndMode2D();

		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
