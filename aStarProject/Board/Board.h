#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#define EPSILON 0.00001


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
	
	typedef enum Direction{
		LEFT, RIGHT, NOT_IMPORTANT
	}Direction;

	void init(std::string boardPath,std::string imagePath, int startCircle, int endCircle, float maxDistFromNeighbour=15, Board::Direction direction=NOT_IMPORTANT);
	void destroy();
	std::vector<Circle*> getCircles(){ return circles;};	
	std::vector<Circle*>& getCirclesRef(){ return circles;};

	void removeCircles(float minR, float maxR=-1, int leftX=-1, int rightX=-1, int bottomY=-1, int topY=-1);

	int getStartCircle(){ return startCircle; };
	int getEndCircle(){ return endCircle; };
	int getMaxRadius(){ return maxRadius; };
	std::vector<int> getSolution();
	std::string imageFilePath;
	int startCircle,endCircle;
	
	float maxDistFromNeighbour;
	
	Board::Direction direction;
	float getHeuristic(Circle* n1);
	std::map<int,Circle*> indToCircle;


private:
	int height, width;
	float maxRadius;
	std::vector<Circle*> circles;
	
	
	
	
};


#endif