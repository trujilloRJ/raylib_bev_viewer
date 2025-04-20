#ifndef INTERFACES
#define INTERFACES

#include <unordered_map>

#define MAX_NUM_DET (500)

using namespace std;

typedef struct detection {
	float posX;
	float posY;
	float doppler;
	int quality;
} Detection;

typedef std::vector<Detection> DetectionList;

typedef unordered_map<int, DetectionList> DetectionMap;

class State {
public:
	State(DetectionMap& detMap, int lastFrame) :
		detMap(detMap), currentFrame(detMap.begin()->first), lastFrame(lastFrame), showCommands(false),
		detSelected(false)
	{};

	DetectionList sliceDetections() { return detMap.at(currentFrame); };

	const int getFirstFrame() { return detMap.begin()->first; }
	const int getLastFrame() { return lastFrame; }
	const int getCurrentFrame() { return currentFrame; }
	const bool isShowCommands() { return showCommands; }

	void toggleShowCommands() { showCommands = !showCommands; }
	void increaseFrames(int value);
	void decreaseFrames(int value);
	bool detSelected;

private:
	int currentFrame;
	int lastFrame;
	bool showCommands;
	DetectionMap detMap;
};

#endif // INTERFACES
