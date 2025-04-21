#include "csv.h"

FILE* CsvReader::openFilename(char* filename)
{
    FILE* fp;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        std::cout << "ERROR: Unable to read file" << filename << '\n';
        throw std::runtime_error("ERROR");
    }

    return fp;
}

void CsvReader::readDetections(char* detFilename, Detections& dets)
{
    FILE* fp = openFilename(detFilename);
    char row[ROW_MAX_CHAR];
    char* token;
    int frame;

    int rowCounter = 0;
    while (feof(fp) != true)
    {
        int colCounter = 0;
        fgets(row, ROW_MAX_CHAR, fp);

        if (rowCounter > 0)
        {
            Detection curDet = { 0., 0., 0., 0 };
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
            if (dets.detections.find(frame) == dets.detections.end())
            {
                std::vector<Detection> detList;
                detList.push_back(curDet);
                dets.detections.insert({ frame, detList });
            }
            else
            {
                dets.detections.at(frame).push_back(curDet);
            }
        }

        rowCounter++;
    }
    dets.lastFrame = frame;

    fclose(fp);
}

void CsvReader::readTracks(char* tFilename, Tracks& trks)
{
    FILE* fp = openFilename(tFilename);
    char row[ROW_MAX_CHAR];
    char* token;
    int frame;

    int rowCounter = 0;
    while (feof(fp) != true)
    {
        int colCounter = 0;
        fgets(row, ROW_MAX_CHAR, fp);

        if (rowCounter > 0)
        {
            Track curTrk = { 0 };
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
                    curTrk.posX = atof(token);
                    break;
                case 2:
                    curTrk.posY = atof(token);
                    break;
                case 3:
                    curTrk.velX = atof(token);
                    break;
                case 4:
                    curTrk.velY = atof(token);
                    break;
                case 5:
                    curTrk.length = atof(token);
                    break;
                case 6:
                    curTrk.width = atof(token);
                    break;
                case 7:
                    curTrk.heading_rad = atof(token);
                    break;
                case 8:
                    curTrk.id = atoi(token);
                    break;
                default:
                    break;
                }
                token = strtok(NULL, ",");
                colCounter++;
            }

            // Add detection to map
            if (trks.tracks.find(frame) == trks.tracks.end())
            {
                std::vector<Track> trkList;
                trkList.push_back(curTrk);
                trks.tracks.insert({ frame, trkList });
            }
            else
            {
                trks.tracks.at(frame).push_back(curTrk);
            }
        }

        rowCounter++;
    }
    trks.lastFrame = frame;

    fclose(fp);
}