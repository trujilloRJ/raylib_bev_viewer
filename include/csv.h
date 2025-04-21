#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

#include "interfaces.h"

constexpr int ROW_MAX_CHAR = 1024;

class CsvReader
{
public:
	void readDetections(char* detFilename, Detections& dets);
	void readTracks(char* tFilename, Tracks& trks);
private:
	FILE* openFilename(char* filename);
};

#endif // !CSV_H
