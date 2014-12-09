#ifndef _CIRCLE_EXTRACTOR_H
#define _CIRCLE_EXTRACTOR_H

#include "Board.h"


std::vector<Circle*> getCirclesFromImage(std::string path, 
			double dp=1, double minDist=6, double param1=2, double param2=13, int minRadius=2, int maxRadius=17);

void printCircles(std::vector<Circle*> circles,std::string imPath);

void extractNeigbours(std::vector<Circle*> &circleVect);





#endif