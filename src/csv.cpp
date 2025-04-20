#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>
#include "csv.h"

#define MAXCHAR 1000

void readDetections(char* detFilename, DetectionMap& detMap, int& lastFrame)
{
    FILE* fp;
    char row[MAXCHAR];
    char* token;
    int frame;

    fp = fopen(detFilename, "r");

    int rowCounter = 0;
    while (feof(fp) != true)
    {
        int colCounter = 0;
        fgets(row, MAXCHAR, fp);
        
        if (rowCounter > 0)
        {
            Detection curDet = {0., 0., 0., 0};
            // iterate over columns
            token = strtok(row, ",");
            while (token != NULL)
            {
                switch (colCounter)
                {
                    case 0:
                        frame = atoi(token);
                        break;
                    case 1:
                        curDet.posX = atof(token);
                        break;
                    case 2:
                        curDet.posY = atof(token);
                        break;
                    case 3:
                        curDet.doppler = atof(token);
                        break;
                    case 4:
                        curDet.quality = atoi(token);
                        break;
                    default:
                        break;
                }
                token = strtok(NULL, ",");
                colCounter++;
            }

            // Add detection to map
            if (detMap.find(frame) == detMap.end())
            {
                std::vector<Detection> detList;
                detList.push_back(curDet);
                detMap.insert({ frame, detList });
            }
            else
            {
                detMap.at(frame).push_back(curDet);
            }
        }

        rowCounter++;
    }
    lastFrame = frame;

    fclose(fp);
}