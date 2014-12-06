#ifndef _CIRCLE_EXTRACTOR_H
#define _CIRCLE_EXTRACTOR_H

#include "Board.h"


std::vector<Circle*> getCirclesFromImage(std::string path);

void printCircles(std::vector<Circle*> circles,std::string imPath);

void extractNeigbours(std::vector<Circle*> &circleVect);





#endif