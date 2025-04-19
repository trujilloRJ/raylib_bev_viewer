#ifndef DRAW
#define DRAW

#include "raylib.h"
#include "interfaces.h"

#define WINDOW_WIDTH (600)
#define WINDOW_HEIGHT (800)
#define WINDOW_HALF_W (WINDOW_WIDTH/2)
#define WINDOW_HALF_H (WINDOW_HEIGHT/2)

#define AXIS_X_RANGE_M (100.0f)
#define AXIS_Y_RANGE_M (200.0f)

#define BG_COLOR BLACK
#define AXIS_COLOR ((Color) { 120, 120, 120, 255 })
#define DET_RADIUS (5)

void drawDetections(DetectionList* pDetList);
void drawAxis();
void transformXYToPixel(float x, float y, int* x_pixel, int* y_pixel);
void transformPixelToXY(int x_pixel, int y_pixel, float* x, float* y);

#endif // !DRAW
