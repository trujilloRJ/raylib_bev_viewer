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

void handleMouseHover(Camera2D& camera, State& state, DetectionList& curDets, TrackList& curTrks)
{
	int x_pixel, y_pixel, index;
	float wp, lp;
	bool collideWithTrk = false;
	bool collideWithDet = false;
	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

	// Check hover on tracks
	if (curTrks.size() > 0) {
		for (index = 0; index < curTrks.size(); index++)
		{
			transformXYToPixel(curTrks[index].posX, curTrks[index].posY, &x_pixel, &y_pixel);
			transformWLToPixel(curTrks[index].width, curTrks[index].length, &wp, &lp);
			collideWithTrk = CheckCollisionPointRec(mousePos, {(float)x_pixel, (float)y_pixel, wp, lp});
			if (collideWithTrk) {
				state.trkSelIndex = index;
				break;
			}
		}
		state.b_trkSelected = collideWithTrk;
	}

	// Check hover on detections, only if trk is not hover
	if ((!collideWithTrk) && (curDets.size() > 0)) {
		for (index = 0; index < curDets.size(); index++)
		{
			transformXYToPixel(curDets[index].posX, curDets[index].posY, &x_pixel, &y_pixel);
			collideWithDet = CheckCollisionPointCircle(mousePos, { (float)x_pixel, (float)y_pixel }, DET_RADIUS);
			if (collideWithDet) {
				state.detSelIndex = index;
				break;
			}
		}
		state.b_detSelected = collideWithDet;
	}

}

void handleIncrementFrame(State& state, KeyboardKey key, int frameDelta, int& frameCounter)
{
	if (IsKeyPressed(key))
	{
		frameCounter = 0;
		state.increaseFrames(frameDelta);
		state.b_detSelected = false;
	}
	if (IsKeyDown(key)) {
		// introduce a delay to prevent too fast frame cycling
		if (framesToMs(frameCounter, FPS) > 500.0) state.increaseFrames(frameDelta);
	}
}

void handleDecreaseFrame(State& state, KeyboardKey key, int frameDelta, int& frameCounter)
{
	if (IsKeyPressed(key))
	{
		frameCounter = 0;
		state.decreaseFrames(frameDelta);
		state.b_detSelected = false;
	}
	if (IsKeyDown(key)) {
		// introduce a delay to prevent too fast frame cycling
		if (framesToMs(frameCounter, FPS) > 500.0) state.decreaseFrames(frameDelta);
	}
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	//SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BEV Viewer");
	SetTargetFPS(FPS);
	Camera2D camera = { 0 };
	camera.zoom = 1.0f;

	/* Initialization */
	DetectionList curDets; 
	Detections dets;
	TrackList curTrks;
	Tracks trks;
	CsvReader reader;
	reader.readDetections("gen_dets.csv", dets);
	//reader.readDetections("sample_data.csv", dets);
	//reader.readTracks("sample_tracks.csv", trks);
	reader.readTracks("gen_tracks.csv", trks);
	int frameCounter = 0;
	State state = State(dets.detections.begin()->first, dets.lastFrame);

	// game loop
	while (!WindowShouldClose())
	{
		frameCounter++;

		/* Event handling */
		handleCameraEvents(&camera);

		handleMouseHover(camera, state, curDets, curTrks);

		if (IsKeyPressed(KEY_H)) state.toggleShowCommands();

		handleIncrementFrame(state, KEY_D, 1, frameCounter);
		handleIncrementFrame(state, KEY_W, 10, frameCounter);		
		handleDecreaseFrame(state, KEY_A, 1, frameCounter);
		handleDecreaseFrame(state, KEY_Q, 10, frameCounter);

		/* Update state */
		curDets = state.sliceDetections(dets.detections);
		if (state.getCurrentFrame() <= trks.lastFrame)
			curTrks = state.sliceTracks(trks.tracks);
		else
			curTrks.clear();

		/* Drawing */
		BeginDrawing();

			ClearBackground(BG_COLOR);

			BeginMode2D(camera);

				drawAxis();

				drawDetections(curDets, state.b_detSelected, state.detSelIndex);

				if (curTrks.size() > 0) drawTracks(curTrks, state.b_trkSelected, state.trkSelIndex);

				//float x = 200.0; float y = 100.0;
				//float w = 100.; float h = 50.;
				//float angle = 170.;
				//float cx = x + w / 2;
				//float cy = y + h / 2;
				//float xr, yr, cxr, cyr, wr, hr;
				//rotateXY(w, h, angle * DEG2RAD, wr, hr);
				//cxr = x + wr/2; cyr = y + hr/2;
				//DrawRectanglePro({ x - (cxr - cx), y - (cyr - cy), w, h}, {0.f, 0.f}, angle, MAROON);


			EndMode2D();

			// Outside of camera context, so it will not be affected by zoom
			drawInfoText(state);

			// Drawing Tooltips
			Vector2 localMousePos = GetMousePosition();
			float x0 = localMousePos.x + 5;
			float y0 = localMousePos.y + 5;
			if (state.b_trkSelected) drawTooltipTrk(curTrks[state.trkSelIndex], x0, y0);
			if (state.b_detSelected) drawTooltipDet(curDets[state.detSelIndex], x0, y0);

		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
