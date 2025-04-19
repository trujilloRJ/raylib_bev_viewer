#ifndef INTERFACES
#define INTERFACES

#define MAX_NUM_DET (500)

typedef struct detection {
	float posX;
	float posY;
} Detection;

typedef struct detectionList {
	int numValidDets;
	Detection detections[MAX_NUM_DET];
} DetectionList;

typedef struct state {
	DetectionList* pDetList;
} State;

#endif // INTERFACES
