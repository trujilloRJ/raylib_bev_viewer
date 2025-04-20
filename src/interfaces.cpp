#include "interfaces.h"

void State::increaseFrames(int value)
{
	int nextFrame = currentFrame + value;
	currentFrame = (nextFrame > lastFrame) ? getFirstFrame() : nextFrame;
}

void State::decreaseFrames(int value)
{
	int nextFrame = currentFrame - value;
	currentFrame = (nextFrame < getFirstFrame()) ? lastFrame : nextFrame;
}
