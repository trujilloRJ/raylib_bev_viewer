#include "interfaces.h"

void State::increaseFrames(int value)
{
	int nextFrame = currentFrame + value;
	currentFrame = (nextFrame > lastFrame) ? firstFrame : nextFrame;
}

void State::decreaseFrames(int value)
{
	int nextFrame = currentFrame - value;
	currentFrame = (nextFrame < firstFrame) ? lastFrame : nextFrame;
}
