
#include <set>
#include <math.h>
#include "Board.h"
#include <map>
#include <queue>
#include <functional>
#include <assert.h>
#include "FileReader.h"



Circle::Circle(int x_p, int y_p, int index_p, int radius_p, float R_p, float G_p, float B_p) : x(x_p), y(y_p),index(index_p),radius(radius_p),R(R_p),G(G_p),B(B_p){

}

float Circle::dist(Circle circle1, Circle circle2){
	float retVal = sqrt((circle1.x-circle2.x)*(circle1.x-circle2.x) + (circle1.y-circle2.y)*(circle1.y-circle2.y))-circle1.radius-circle2.radius;

	return retVal;
	
}




void Board::removeCircles(float minR, float maxR, int leftX, int rightX, int bottomY, int topY){
	for(int i=0 ; i< this->circles.size() ; i++){
		circles[i]->neighbours = std::vector<int>();
		if(circles[i]->radius < minR){
			circles.erase(circles.begin()+i);
			i--;
		}
	}
	
	
}


void Board::init(std::string boardPath,std::string imagePath, int startCircle, int endCircle, float maxDistFromNeighbour, Board::ConstraintType constraint){
	circles = readBoardFromFile(boardPath);
	maxRadius = -1;
	for(int i=0 ; i<circles.size() ; i++){
		maxRadius = maxRadius > circles[i]->radius ? maxRadius : circles[i]->radius;
	}

	imageFilePath = imagePath;
	
	for(int i=0 ; i<circles.size() ; i++){
		Circle* nextCircle = circles[i];
		assert(nextCircle->index == i);
		indToCircle.insert(std::pair<int,Circle*>(nextCircle->index,nextCircle));
	}

	this->maxDistFromNeighbour = maxDistFromNeighbour;

	this->startCircle = startCircle;
	this->endCircle = endCircle;
	this->constraintType = constraintType;
}

void Board::destroy(){
	for(int i=0 ; i<circles.size() ; i++){
		delete circles[i];
	}

}

std::vector<int> Board::getSolution(){
	return std::vector<int>();	
}


inline bool Circle::operator==(const Circle& other){
	return this->index == other.index;
}
	

int getEdgeWeight(Circle* n1,Circle* n2){
	if(*n1 == *n2){
		return 0;
	}
	return 1;
}

float Board::getHeuristic(Circle* n1){
	Circle* end = indToCircle[endCircle];
	return Circle::dist(*n1,*end)/(maxRadius*2+maxDistFromNeighbour);
}




int findMinIndexFromOpenSet(std::set<int> openSet, std::vector<float> f_score){
	float minFScore = 100000000;
	int minIndex = -1;

	std::set<int>::iterator it;
	for (it = openSet.begin(); it != openSet.end(); ++it){
		if (f_score[*it] < minFScore){
			minIndex = *it;
			minFScore = f_score[*it];
		}
	}
	return minIndex;
}

