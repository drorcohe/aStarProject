
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

void Board::init(std::string boardPath,std::string imagePath,float maxDistFromNeighbour){
	circles = readBoardFromFile(boardPath,startCircle,endCircle);
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

float Board::getHeuristic(Circle* n1){
	Circle* end = indToCircle[endCircle];
	return Circle::dist(*n1,*end)/(maxRadius*2+maxDistFromNeighbour);
}




std::vector<int> reconstruct_path(std::vector<int> cameFrom,int current){
	std::vector<int> path;
	while(current != -1){
		path.push_back(current);
		current = cameFrom.at(current);
	}

	return path; //TODO - reverse the order
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


std::vector<int> Board::aStarSearch(){

	std::vector<Circle*> board = this->getCircles();
	int start = this->startCircle;
	int end = this->endCircle;

	std::set<int> closeSet; // The set of nodes already evaluated.

	 // The set of tentative nodes to be evaluated, initially containing the start node
	//std::priority_queue<int, std::vector<int>, std::greater<int>> openSetQueue;
	std::set<int> openSet;
	openSet.insert(start);
	

	std::vector<int> cameFrom(board.size());  ; //a map of navigated nodes
	for(int i=0 ; i< board.size() ; i++){
		cameFrom[i] = -1;
	}

	std::vector<float> g_score(board.size());  
	g_score[start] = 0; // Cost from start along best known path.
	//std::vector<float> tenative_g_score(board.size()); // Cost from start along best known path.
	 
	
	//Estimated total cost from start to goal through y.
	std::vector<float> f_score(board.size());  // Cost from start along best known path.
	f_score[start] = g_score[start] + this->getHeuristic(indToCircle[start]);
	
	while(openSet.size() > 0){
		
		
		int currentCircleIndex = findMinIndexFromOpenSet(openSet,f_score); //the node in openset having the lowest f_score[] value
		Circle* currentCircle = indToCircle[currentCircleIndex];
		if(currentCircle == indToCircle[endCircle]){
			return reconstruct_path(cameFrom, endCircle);
		}


		openSet.erase(currentCircle->index);
		closeSet.insert(currentCircle->index);

		for(std::vector<int>::iterator neighbour = currentCircle->neighbours.begin(); neighbour != currentCircle->neighbours.end(); ++neighbour) {
			//if the neighbour is already in the close set - continue
			if(closeSet.count(*neighbour)!=0){
				continue;
			}
			Circle* neigbourCircle = indToCircle[*neighbour];
			float tentative_g_score = g_score[currentCircleIndex] + 1;
			if(openSet.count(*neighbour)==0 || tentative_g_score < g_score[*neighbour]){
				cameFrom[*neighbour] = currentCircle->index;
				g_score[*neighbour] = tentative_g_score;
				f_score[*neighbour] = g_score[*neighbour] + getHeuristic(neigbourCircle);
				if(openSet.count(*neighbour)==0){
					openSet.insert(*neighbour);
				}
			}

		}

	}

	assert(0==1);

	return std::vector<int>();
	
	
}

