#include "AStarSolver.h"
#include <set>
#include <math.h>
#include "Board.h"
#include <map>
#include <queue>
#include <functional>
#include <assert.h>
#include "FileReader.h"
#include <algorithm>


void AStarSolver::init(Board& board, int startCircle, int endCircle,  AStarSolver::Direction direction){
	this->board = board;
	this->startCircle = startCircle;
	this->endCircles = std::vector<int>();
	this->endCircles.push_back(endCircle);
	this->targetSpinDirections = std::vector<AStarSolver::Direction>();
	this->targetSpinDirections.push_back(direction);
}

void AStarSolver::init(Board& board, int startCircle, std::vector<int> endCircles, std::vector<AStarSolver::Direction> directions){
	this->board = board;
	this->startCircle = startCircle;
	this->endCircles = std::vector<int>(endCircles);
	this->targetSpinDirections = std::vector<AStarSolver::Direction>(directions);
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




std::vector<int> reconstruct_path(std::vector<int> cameFrom,int current){
	std::vector<int> path;
	while(current != -1){
		path.push_back(current);
		current = cameFrom.at(current);
	}

	return path; //TODO - reverse the order
}

std::vector<int> AStarSolver::solve(){
	std::sort (endCircles.begin(),endCircles.end());

	int bestPath  = INT_MAX;
	std::vector<int> bestPathSoFar = std::vector<int>();
	
	do {
		bool invalidPath = false;
		AStarSolver::Direction previousDirection = RIGHT; //first circle always goes right
		std::vector<int> currentPath = std::vector<int>();
		int subPathStartCircle = startCircle;

		for(int i=0 ; i< endCircles.size() ; i++){

			assert(targetSpinDirections[i]!=NOT_IMPORTANT);

			AStarSolver::PathType pathType = previousDirection == targetSpinDirections[i]? EVEN : ODD;
			std::vector<int> bestSubPath = aStarSearch(subPathStartCircle, endCircles[i],pathType,currentPath);
			if(bestSubPath.size()==0){
				invalidPath = true;
				break;
			}
			currentPath.insert(currentPath.end(),bestSubPath.begin(),bestSubPath.end()-1);

			assert(bestSubPath.size() > 0 && ((bestSubPath.size()%2 == 0 && pathType==EVEN) ||
				(bestSubPath.size()%2 == 1 && pathType==ODD) || pathType==TYPE_NOT_IMPORTANT));

			subPathStartCircle = endCircles[i];
			previousDirection = targetSpinDirections[i];
		}

		if(invalidPath){
			continue;
		}
		currentPath.push_back(endCircles[endCircles.size()-1]);
		if(currentPath.size()<bestPath){
			bestPathSoFar = currentPath;
			bestPath = currentPath.size();
		}
	} while ( std::next_permutation(endCircles.begin(),endCircles.end()) );


#ifdef DEBUG
	int numOfFoundedTargets = 0;

	for(int i=0 ; i<endCircles.size() ; i++){
		for(int j=0 ; j<bestPathSoFar.size() ; j++){
			if(endCircles[i]==bestPathSoFar[j]){
				numOfFoundedTargets++;
				break;
			}
		}
	}
	assert(numOfFoundedTargets==endCircles.size());
#endif
	return bestPathSoFar;

}

std::vector<int> AStarSolver::aStarSearch(int start,int end, AStarSolver::PathType pathType, std::vector<int> invalidIndices){

	std::vector<Circle*> circles = board.getCircles();

	std::set<int> closeSet; // The set of nodes already evaluated.

	 // The set of tentative nodes to be evaluated, initially containing the start node
	//std::priority_queue<int, std::vector<int>, std::greater<int>> openSetQueue;
	std::set<int> openSet;
	openSet.insert(start);
	

	std::vector<int> cameFrom(circles.size());  ; //a map of navigated nodes
	for(int i=0 ; i< circles.size() ; i++){
		cameFrom[i] = -1;
	}

	std::vector<float> g_score(circles.size());  
	g_score[start] = 0; // Cost from start along best known path.
	//std::vector<float> tenative_g_score(board.size()); // Cost from start along best known path.
	 
	
	//Estimated total cost from start to goal through y.
	std::vector<float> f_score(circles.size());  // Cost from start along best known path.
	f_score[start] = g_score[start] + getHeuristic(board.indToCircle[start],end);
	
	while(openSet.size() > 0){
		
		
		int currentCircleIndex = findMinIndexFromOpenSet(openSet,f_score); //the node in openset having the lowest f_score[] value
		Circle* currentCircle = board.indToCircle[currentCircleIndex];
		if(currentCircle == board.indToCircle[end]){
			std::vector<int> retPath = reconstruct_path(cameFrom, end);
			return retPath;
		}


		openSet.erase(currentCircle->index);
		closeSet.insert(currentCircle->index);

		for(std::vector<int>::iterator neighbour = currentCircle->neighbours.begin(); neighbour != currentCircle->neighbours.end(); ++neighbour) {
			//if the neighbour is already in the close set - continue
			if(closeSet.count(*neighbour)!=0){
				continue;
			}
			Circle* neigbourCircle = board.indToCircle[*neighbour];

			//if this is the endCircle - checks if out path is even or odd before continues.
			std::vector<int> pathSoFar = reconstruct_path(cameFrom, currentCircle->index);
			if(neigbourCircle->index==end && pathType!=NOT_IMPORTANT){
				if((pathSoFar.size() % 2 == 0 && pathType==EVEN) || (pathSoFar.size() % 2 == 1 && pathType==ODD) ){
					continue;
				}
			}

			//if we are not allowed to go through this circle - continue regularly
			bool isInvalidNeigbour = false;
			for(int t=0 ; t< invalidIndices.size() ;t++){
				isInvalidNeigbour |= (invalidIndices[t]==neigbourCircle->index);
			}
			if(isInvalidNeigbour){
				continue;
			}
			


			float tentative_g_score = g_score[currentCircleIndex] + 1;
			if(openSet.count(*neighbour)==0 || tentative_g_score < g_score[*neighbour]){
				cameFrom[*neighbour] = currentCircle->index;
				g_score[*neighbour] = tentative_g_score;
				f_score[*neighbour] = g_score[*neighbour] + getHeuristic(neigbourCircle, end);
				if(openSet.count(*neighbour)==0){
					openSet.insert(*neighbour);
				}
			}

		}

	}

	if(invalidIndices.size()==0){
		assert(0==1);
	}
	

	return std::vector<int>();
	
	
}



float AStarSolver::getHeuristic(Circle* n1, int endCircle){
	Circle* end = board.indToCircle[endCircle];
	return Circle::dist(*n1,*end)/(board.maxRadius*2+board.maxDistFromNeighbour);
}
