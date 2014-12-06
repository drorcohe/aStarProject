#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#define EPSILON 0.00001
#define MAX_DIST_FROM_NEIGHBOUR 15

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
	void init(std::string boardPath,std::string imagePath);
	void destroy();
	std::vector<Circle*> getCircles(){ return circles;};	
	int getStartCircle(){ return startCircle; };
	int getEndCircle(){ return endCircle; };
	int getMaxRadius(){ return maxRadius; };
	std::vector<int> getSolution();
	std::string imageFilePath;
	int startCircle,endCircle;
	std::vector<int> aStarSearch();

private:
	
	float maxRadius;
	std::vector<Circle*> circles;
	std::map<int,Circle*> indToCircle;
	float getHeuristic(Circle* n1);
	
	
};


#endif