#ifndef _BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>


class Circle{

public:
	int x,y;
	int radius;
	std::vector<int> neighbours;
	float R,G,B; //from 0 to 1
};



class Board{
public:
	void init(std::string boardPath,std::string imagePath);
	std::vector<Circle> getBoard(){ return board;};	
	int getStartCircle(){ return startCircle; };
	int getEndCircle(){ return endCircle; };
	std::vector<int> getSolution();

private:
	int startCircle,endCircle;
	std::vector<Circle> board;
	
};


#endif