#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include "SharedTypes.h"
#include <map>
#define EPSILON 0.00001



typedef struct PATH_DETAIL{
	int start;
	std::vector<int> endPoints;
	std::vector<Direction> directions;
}PATH_DETAIL;


class Circle{

public:
	Circle(int x, int y, int index, int radius, float R, float G, float B);

	int x,y;
	int index;
	int radius;
	std::vector<int> neighbours;
	int R,G,B; //from 0 to 1

	inline bool operator==(const Circle& other);



	static float dist(Circle circle1, Circle circle2);
	
};



class Board{
public:


	void init(std::string boardPath,std::string imagePath, float maxDistFromNeighbour=3.5);
	void destroy();
	std::vector<Circle*> getCircles(){ return circles;};	
	std::vector<Circle*>& getCirclesRef(){ return circles;};

	void removeCircles(float minR, float maxR=-1, int leftX=-1, int rightX=-1, int bottomY=-1, int topY=-1);

	int getMaxRadius(){ return maxRadius; };
	std::vector<int> getSolution();
	std::string imageFilePath;

	
	float maxDistFromNeighbour;
	
	
	std::string boardFileName;
	std::map<int,Circle*> indToCircle;
	float maxRadius;
	int height, width;
	std::vector<PATH_DETAIL> recommendedPaths;
	


private:
	std::vector<Circle*> circles;
	
	
	
};


#endif