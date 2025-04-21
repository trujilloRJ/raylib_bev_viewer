#ifndef INTERFACES
#define INTERFACES

//#include <unordered_map>
#include <vector>
#include <map>

using namespace std;

typedef struct detection {
	float posX;
	float posY;
	float doppler;
	int quality;
} Detection;
typedef vector<Detection> DetectionList;
typedef map<int, DetectionList> DetectionMap;
typedef struct detections {
	int lastFrame;
	DetectionMap detections;
} Detections;

typedef struct track {
	int id;
	float posX;
	float posY;
	float velX;
	float velY;
	float heading_rad;
	float width;
	float length;
} Track;
typedef vector<Track> TrackList;
typedef map<int, TrackList> TrackMap;
typedef struct tracks {
	int lastFrame;
	TrackMap tracks;
} Tracks;

class State {
public:
	State(int firstFrame, int lastFrame) :
		firstFrame(firstFrame), lastFrame(lastFrame), currentFrame(firstFrame), showCommands(false),
		b_detSelected(false), detSelIndex(0), b_trkSelected(false), trkSelIndex(0)
	{};

	DetectionList sliceDetections(DetectionMap& detMap) { return detMap.at(currentFrame); };
	TrackList sliceTracks(TrackMap& tMap) { return tMap.at(currentFrame); };

	const int getCurrentFrame() { return currentFrame; }
	const bool isShowCommands() { return showCommands; }

	void toggleShowCommands() { showCommands = !showCommands; }
	void increaseFrames(int value);
	void decreaseFrames(int value);
	bool b_detSelected;
	bool b_trkSelected;
	int trkSelIndex;
	int detSelIndex;

private:
	int firstFrame;
	int lastFrame;
	int currentFrame;
	bool showCommands;
};

#endif // INTERFACES
