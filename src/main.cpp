//#include <stdio.h>
#include <string.h>
#include <iostream>
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "interfaces.h"
#include "draw.h"
#include "csv.h"
#include "raylib_utils.h"

constexpr int FPS = 60;

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
		camera->target = {0.0f, 0.0f};
		camera->offset = {0.0f, 0.0f};
	}
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BEV Viewer");
	SetTargetFPS(FPS);
	Camera2D camera = { 0 };
	camera.zoom = 1.0f;

	/* Initialization */
	DetectionMap detMap;
	int lastFrame;
	readDetections("sample_data.csv", detMap, lastFrame);
	State state = State(detMap, lastFrame);
	int frameCounter = 0;

	// game loop
	while (!WindowShouldClose())
	{
		frameCounter++;

		/* Event handling */
		handleCameraEvents(&camera);
		if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D))
		{
			// introduce a delay to prevent too fast frame cycling
			if (framesToMs(frameCounter, FPS) > 120.0)
			{
				state.increaseFrames(1); 
				frameCounter = 0;
			}
		}
		if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A))
		{
			if (framesToMs(frameCounter, FPS) > 120.0)
			{
				state.decreaseFrames(1);
				frameCounter = 0;
			}
		}

		/* Update state */
		const DetectionList* pCurDets = state.sliceDetections();

		/* Drawing */
		BeginDrawing();

			ClearBackground(BG_COLOR);

			BeginMode2D(camera);

				drawDetections(*(pCurDets));
				drawAxis();
				drawInfoText(state);

			EndMode2D();

		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
