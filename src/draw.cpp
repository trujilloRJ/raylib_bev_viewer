#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "draw.h"

void drawDetections(DetectionList& detList, bool hoverDet, int selectedDetIndex)
{
	int x_pix, y_pix, i;

	for (i = 0; i < detList.size(); i++)
	{
		Detection curDet = detList[i];
		Color color = (curDet.quality == 1) ? GREEN : BAD_DET_COLOR;
		float radius = (hoverDet && (i == selectedDetIndex)) ? DET_RADIUS + 3 : DET_RADIUS;
		transformXYToPixel(curDet.posX, curDet.posY, &x_pix, &y_pix);
		DrawCircle(x_pix, y_pix, radius, color);
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
	Font font = GetFontDefault();
	char frameText[15];
	sprintf(frameText, "FRAME: %d", state.getCurrentFrame());
	DrawTextEx(font, frameText, { 5, 10 }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);

	if (state.isShowCommands())
	{
		DrawTextEx(font, "H: HIDE COMMANDS", { 5, 10 + INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
		DrawTextEx(font, "A/D: +/- 1 FRAME", { 5, 10 + 2 * INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
		DrawTextEx(font, "Q/W: +/- 10 FRAMES", { 5, 10 + 3 * INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
		DrawTextEx(font, "R: RESET VIEW", { 5, 10 + 4 * INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
		DrawTextEx(font, "RIGHT CLIK: ZOOM", { 5, 10 + 5 * INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
	}
	else
	{
		DrawTextEx(font, "H: SHOW COMMANDS", { 5, 10 + INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
	}
}

void drawTooltipDet(Detection& det, float initialX, float initialY) {
	Font font = GetFontDefault();
	char floatText[15];

	DrawRectangleRec({ initialX, initialY, 110, 90 }, { 50, 50, 50, 150 });
	initialX += 10; 
	initialY += 10;

	//DrawTextEx(font, "DETECTION", { initialX, initialY }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
	sprintf(floatText, "x: %.1f m", det.posX);
	DrawTextEx(font, floatText, { initialX, initialY}, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
	sprintf(floatText, "y: %.1f m", det.posY);
	DrawTextEx(font, floatText, { initialX, initialY + 1 * INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
	sprintf(floatText, "v: %.1f m/s", det.doppler);
	DrawTextEx(font, floatText, { initialX, initialY + 2 * INFO_Y_SPACING }, INFO_FONT_SIZE, INFO_FONT_SPACING, FONT_COLOR);
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
