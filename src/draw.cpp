#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "draw.h"

void drawDetections(const DetectionList& detList)
{
	int x_pix, y_pix;

	for (Detection curDet : detList)
	{
		Color color = (curDet.quality == 1) ? GREEN : MAROON;
		transformXYToPixel(curDet.posX, curDet.posY, &x_pix, &y_pix);
		DrawCircle(x_pix, y_pix, DET_RADIUS, color);
	}
}

void drawAxis()
{
	int iter;
	int xTickPos, yTickPos;
	float xTickValue, yTickValue;
	int nTicks = 10;
	char tickValueText[10];
	
	DrawLine(WINDOW_HALF_W, 0, WINDOW_HALF_W, WINDOW_HEIGHT, AXIS_COLOR);
	DrawLine(0, WINDOW_HALF_H, WINDOW_WIDTH, WINDOW_HALF_H, AXIS_COLOR);

	/* set axis ticks */
	for (iter = 0; iter < nTicks; iter++)
	{
		xTickPos = iter * WINDOW_WIDTH / nTicks;
		yTickPos = iter * WINDOW_HEIGHT / nTicks;
		DrawLine(xTickPos, WINDOW_HALF_H - 4, xTickPos, WINDOW_HALF_H + 4, AXIS_COLOR);
		DrawLine(WINDOW_HALF_W - 4, yTickPos, WINDOW_HALF_W + 4, yTickPos, AXIS_COLOR);

		/* drawing axis value */
		transformPixelToXY(xTickPos, yTickPos, &xTickValue, &yTickValue);
		sprintf(tickValueText, "%.1f", xTickValue);
		DrawText(tickValueText, xTickPos - 6, WINDOW_HALF_H + 5, 11, AXIS_COLOR);
		if (fabs(yTickValue) > 1.0f) // prevent drawing zero two times
		{
			sprintf(tickValueText, "%.1f", yTickValue);
			DrawText(tickValueText, WINDOW_HALF_W + 5, yTickPos - 6, 11, AXIS_COLOR);
		}
	}
}

void drawInfoText(State& state)
{
	char frameText[15];
	sprintf(frameText, "Frame: %d", state.getCurrentFrame());
	DrawText(frameText, 10, 10, INFO_FONT_SIZE, WHITE);
	DrawText("A/D: Moves between frames", 10, 35, INFO_FONT_SIZE, WHITE);
	DrawText("R: Reset view", 10, 60, INFO_FONT_SIZE, WHITE);
	DrawText("Right click: Zoom", 10, 85, INFO_FONT_SIZE, WHITE);
}

void transformXYToPixel(float x, float y, int* x_pixel, int* y_pixel)
{
	*(x_pixel) = (int)(x * ((float)WINDOW_WIDTH) / AXIS_X_RANGE_M  + WINDOW_HALF_W);
	*(y_pixel) = (int)(y * ((float)WINDOW_HEIGHT) / AXIS_Y_RANGE_M  + WINDOW_HALF_H);
}

void transformPixelToXY(int x_pixel, int y_pixel, float* x, float* y)
{
	*(x) = (float)(x_pixel - WINDOW_HALF_W) * AXIS_X_RANGE_M / (float)WINDOW_WIDTH;
	*(y) = (float)(y_pixel - WINDOW_HALF_H) * AXIS_Y_RANGE_M / (float)WINDOW_HEIGHT;
}
