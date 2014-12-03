
#include <set>
#include <math.h>
#include "Board.h"
#include <map>
#include <queue>
#include <functional>
#include <assert.h>
int main2(){
	return 1;
}


Circle::Circle(int x_p, int y_p, int index_p, int radius_p, float R_p, float G_p, float B_p) : x(x_p), y(y_p),index(index_p),radius(radius_p),R(R_p),G(G_p),B(B_p){

}

void Board::init(std::string boardPath,std::string imagePath){
	//init board and map and maxRadius

}

void Board::destroy(){
	//destroy all
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
	Circle* end = indToCircle.at(endCircle);
	return (std::abs(n1->x - end->x)+std::abs(n1->y - end->y))/maxRadius;
}




std::vector<int> reconstruct_path(std::vector<int> cameFrom,int current){
	std::vector<int> path;
	while(current != -1){
		path.push_back(current);
		current = cameFrom.at(current);
	}

	return path; //TODO - reverse the order
}


std::vector<int> Board::aStarSearch(std::vector<Circle*> board, int start,int end){
	std::set<int> closeSet; // The set of nodes already evaluated.

	 // The set of tentative nodes to be evaluated, initially containing the start node
	//std::priority_queue<int, std::vector<int>, std::greater<int>> openSetQueue;
	std::set<int> openSet;
	openSet.insert(start);
	

	std::vector<int> cameFrom; //a map of navigated nodes

	float* g_score = new float (board.size());  // Cost from start along best known path.
	g_score[start] = 0;
	float* tenative_g_score = new float (board.size());  // Cost from start along best known path.
	
	//Estimated total cost from start to goal through y.
	float* f_score = new float (board.size());  // Cost from start along best known path.
	f_score[start] = g_score[start] + this->getHeuristic(indToCircle.at(start));
	
	while(openSet.size() > 0){
		
		
		int minCircleIndex = *openSet.rbegin();
		Circle* currentCircle = indToCircle.at(minCircleIndex);
		if(currentCircle == indToCircle.at(endCircle)){
				delete[] g_score;
				delete[] f_score;
			return reconstruct_path(cameFrom, endCircle);
		}


		openSet.erase(currentCircle->index);
		closeSet.insert(currentCircle->index);

		for(std::vector<int>::iterator neighbour = currentCircle->neighbours.begin(); neighbour != currentCircle->neighbours.end(); ++neighbour) {
			if(closeSet.count(currentCircle->index)!=0){
				continue;
			}

			if(openSet.count(*neighbour)==0 || tenative_g_score[*neighbour] < g_score[*neighbour]){
				cameFrom[*neighbour] = currentCircle->index;
				g_score[*neighbour] = tenative_g_score[*neighbour];
				f_score[*neighbour] = g_score[*neighbour] + getHeuristic(indToCircle.at(*neighbour));
				if(openSet.count(*neighbour)==0){
					openSet.insert(*neighbour);
				}
			}

		}

	}
	
	delete[] g_score;
	delete[] f_score;
	assert(0==1);

	return std::vector<int>();
	
	
}

