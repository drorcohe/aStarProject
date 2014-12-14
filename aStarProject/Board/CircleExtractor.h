#ifndef _CIRCLE_EXTRACTOR_H
#define _CIRCLE_EXTRACTOR_H

#include "Board.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

typedef struct ParametersSet{
	std::string name,imagePath;
	double dp, minDist, param1, param2;
	int minRadius, maxRadius;


	float maxDistFromNeighbour;

}ParametersSet;


extern ParametersSet HEART_PARAMETER_SET,DEER_PARAMETER_SET,SHOUT_PARAMETER_SET,MERLIN_PARAMETER_SET,MAP_PARAMETER_SET;


std::vector<Circle*> getCirclesFromImage(ParametersSet set);
std::vector<Circle*> getCirclesFromImage(std::string path, 
			double dp=1, double minDist=6, double param1=2, double param2=13, int minRadius=2, int maxRadius=17,float maxDistanceFromNeighbours=15);

cv::Mat printCircles(std::vector<Circle*> circles,std::string imPath);

void extractNeigbours(std::vector<Circle*> &circleVect, float maxDistFromNeigbour = 15, bool addOpt = true);


bool doesCirclesIntersect(float x0, float y0, float r0, float x1, float y1, float r1);

#endif