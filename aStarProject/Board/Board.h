#ifndef _BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#define EPSILON 0.00001

class Circle{

public:
	int x,y;
	int index;
	int radius;
	std::vector<int> neighbours;
	float R,G,B; //from 0 to 1

	inline bool operator==(const Circle& other);
	
};



class Board{
public:
	void init(std::string boardPath,std::string imagePath);
	void destroy();
	std::vector<Circle*> getBoard(){ return board;};	
	int getStartCircle(){ return startCircle; };
	int getEndCircle(){ return endCircle; };
	int getMaxRadius(){ return maxRadius; };
	std::vector<int> getSolution();



private:
	float maxRadius;
	int startCircle,endCircle;
	std::vector<Circle*> board;
	std::map<int,Circle*> indToCircle;
	float getHeuristic(Circle* n1);
	std::vector<int> aStarSearch(std::vector<Circle*> board, int start,int end);
	
};


#endif