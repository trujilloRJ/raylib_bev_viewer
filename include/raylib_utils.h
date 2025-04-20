#ifndef RAYUTILS
#define RAYUTILS

inline float framesToMs(int frames, int fps) {
	return (float)frames / (float)fps * 1000.0f;
}

#endif // !RAYUTILS
