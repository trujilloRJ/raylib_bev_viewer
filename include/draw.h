#ifndef DRAW
#define DRAW

#include "raylib.h"
#include "interfaces.h"
#include <vector>

#define WINDOW_WIDTH (600)
#define WINDOW_HEIGHT (800)
#define WINDOW_HALF_W (WINDOW_WIDTH/2)
#define WINDOW_HALF_H (WINDOW_HEIGHT/2)

//https://colorhunt.co/palette/1d16168e1616d84040eeeeee
//rgb(29, 22, 22)
//rgb(142, 22, 22)
//rgb(216, 64, 64)
//rgb(238, 238, 238)

#define AXIS_X_RANGE_M (100.0f)
#define AXIS_Y_RANGE_M (200.0f)

#define DET_RADIUS (3)

constexpr int INFO_Y_SPACING = 25;
constexpr int INFO_FONT_SIZE = 18;
constexpr float INFO_FONT_SPACING = 2.0;
constexpr Color BG_COLOR = { 29, 22, 22, 255 };
constexpr Color AXIS_COLOR = { 238, 238, 238, 125 };
constexpr Color FONT_COLOR = { 238, 238, 238, 255 };
constexpr Color TOOLTIP_FONT_COLOR = BG_COLOR;
constexpr Color BAD_DET_COLOR = { 216, 64, 64, 255 };

void drawDetections(DetectionList& detList);
void drawInfoText(State& state);
void drawAxis();
void drawTooltipDet(Detection& det, float initialX, float initialY);
void transformXYToPixel(float x, float y, int* x_pixel, int* y_pixel);
void transformPixelToXY(int x_pixel, int y_pixel, float* x, float* y);

#endif // !DRAW
