#include <cstring>
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
	if((circle1.x==circle2.x) && (circle1.y == circle2.y)){
		return 0;
	}
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


void Board::init(std::string boardPath,std::string imagePath, float maxDistFromNeighbour){
	readBoardFromFile(boardPath,circles);
	boardFileName = boardPath;
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




