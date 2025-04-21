#ifndef RAYUTILS
#define RAYUTILS

#include <math.h>

inline float framesToMs(int frames, int fps) {
	return (float)frames / (float)fps * 1000.0f;
}

inline void rotateXY(float x, float y, float angle_rad, float& xr, float& yr) {
	xr = x * cosf(angle_rad) - y * sinf(angle_rad);
	yr = x * sinf(angle_rad) + y * cosf(angle_rad);
}

#endif // !RAYUTILS
