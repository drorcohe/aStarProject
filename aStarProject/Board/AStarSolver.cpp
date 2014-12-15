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





void AStarSolver::init(Board& board, int startCircle, int endCircle,  Direction direction){

	this->board = board;
	this->startCircle = startCircle;
	this->endCircles = std::vector<int>();
	this->endCircles.push_back(endCircle);
	this->targetSpinDirections = std::vector<Direction>();
	this->targetSpinDirections.push_back(direction);
}

void AStarSolver::init(Board& board, int startCircle, std::vector<int> endCircles, std::vector<Direction> directions){
	this->board = board;
	this->startCircle = startCircle;
	this->endCircles = std::vector<int>(endCircles);
	this->targetSpinDirections = std::vector<Direction>(directions);

	//sets max radius for board object
	int maxSoFar = -1;
	for(int i=0 ; i<board.getCircles().size() ; i++){
		maxSoFar = std::max(maxSoFar,board.getCircles()[i]->radius);
	}
}

AStarNode findMinIndexFromOpenSet(std::set<AStarNode> openSet, std::map<AStarNode,float> f_score){
	float minFScore = 100000000;
	AStarNode minIndex;

	std::set<AStarNode>::iterator it;
	
	for (it = openSet.begin(); it != openSet.end(); ++it){
		AStarNode nextNode = *it;
		if (f_score[nextNode] < minFScore){
			minIndex = nextNode;
			minFScore = f_score[nextNode];
		}
	}
	return minIndex;
}




std::vector<int> reconstruct_path(std::map<AStarNode,AStarNode> cameFrom,AStarNode start, AStarNode end){
	std::vector<int> path;
	AStarNode current = end;
	while(current.circleIndex != start.circleIndex){
		path.push_back(current.circleIndex);
		current = cameFrom.at(current);
	}
	path.push_back(start.circleIndex);

	return path;
}

std::vector<int> AStarSolver::solve(){
	std::sort (endCircles.begin(),endCircles.end());

	int bestPath  = INT_MAX;
	std::vector<int> bestPathSoFar = std::vector<int>();
	
	do {
		bool invalidPath = false;
		Direction previousDirection = RIGHT; //first circle always goes right
		std::vector<int> currentPath = std::vector<int>();
		int subPathStartCircle = startCircle;

		for(int i=0 ; i< endCircles.size() ; i++){

			assert(targetSpinDirections[i]!=NOT_IMPORTANT);

			std::vector<AStarNode> visitedNodes;

			if(currentPath.size() > 0){
				for(int i=0 ; i<currentPath.size() ; i++){
					AStarNode nextNode;
					nextNode.circleIndex = currentPath[i];
					nextNode.dir = RIGHT;
					visitedNodes.push_back(nextNode);
					AStarNode nextNode2 = nextNode;
					nextNode2.dir = LEFT;
					visitedNodes.push_back(nextNode2);
				}
				
			}
			std::vector<int> bestSubPath = aStarSearch(subPathStartCircle, endCircles[i],previousDirection,targetSpinDirections[i],visitedNodes);
			if(bestSubPath.size()==0){
				invalidPath = true;
				break;
			}
			currentPath.insert(currentPath.end(),bestSubPath.begin(),bestSubPath.end()-1);

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

	std::set<int> setPath(bestPathSoFar.begin(),bestPathSoFar.end());
	assert(setPath.size() == bestPathSoFar.size());
#endif
	return bestPathSoFar;

}


std::vector<int> AStarSolver::aStarSearch(int start,int end, Direction directionStart,Direction directionEnd , std::vector<AStarNode> visitedNodes){

	std::vector<Circle*> circles = board.getCircles();
	std::set<AStarNode> closeSet; // The set of nodes already evaluated.
	closeSet.insert(visitedNodes.begin(),visitedNodes.end());

	AStarNode endNode = {end,directionEnd};
	 // The set of tentative nodes to be evaluated, initially containing the start node
	//std::priority_queue<int, std::vector<int>, std::greater<int>> openSetQueue;
	std::set<AStarNode> openSet;

	AStarNode startNode = {start,directionStart};
	openSet.insert(startNode);
	
	std::map<AStarNode,AStarNode> cameFrom; //a map of navigated nodes
	
	std::map<AStarNode,float> g_score;  
	g_score.insert(std::pair<AStarNode,int>(startNode,0)); // Cost from start along best known path.

	//Estimated total cost from start to goal through y.
	std::map<AStarNode,float> f_score;   // Cost from start along best known path.
	f_score.insert(std::pair<AStarNode,int>(startNode,g_score[startNode] + getHeuristic(board.indToCircle[start],end)));
	
	while(openSet.size() > 0){
		
		if(openSet.count(endNode) > 0){
			Circle* currentCircle = board.indToCircle[end];
			std::vector<int> retPath = reconstruct_path(cameFrom, startNode, endNode);
			std::reverse(retPath.begin(),retPath.end());
			return retPath;
		}
		AStarNode currentCircleNode = findMinIndexFromOpenSet(openSet,f_score); //the node in openset having the lowest f_score[] value
		Circle* currentCircle = board.indToCircle[currentCircleNode.circleIndex];
		
		if(currentCircleNode.circleIndex==455){
			int b =2;
		}

		openSet.erase(currentCircleNode);
		closeSet.insert(currentCircleNode);


		for(std::vector<int>::iterator neighbour = currentCircle->neighbours.begin(); neighbour != currentCircle->neighbours.end(); ++neighbour) {
			//if the neighbour is already in the close set - continue
			
			Circle* neigbourCircle = board.indToCircle[*neighbour];
			AStarNode neighbourNode = {neigbourCircle->index,(currentCircleNode.dir==LEFT? RIGHT:LEFT)};
			if(closeSet.count(neighbourNode)!=0){
				continue;
			}


			//if this is the endCircle - checks if out path is even or odd before continues.
			if(neighbourNode.circleIndex==end && neighbourNode.dir!=directionEnd ){
				continue;
			}		


			float tentative_g_score = g_score[currentCircleNode] + 1;
			if(openSet.count(neighbourNode)==0 || tentative_g_score < g_score[neighbourNode]){
				cameFrom[neighbourNode] = currentCircleNode;
				g_score[neighbourNode] = tentative_g_score;
				f_score[neighbourNode] = g_score[neighbourNode] + getHeuristic(neigbourCircle, end);
				if(openSet.count(neighbourNode)==0){
					openSet.insert(neighbourNode);
				}
			}
			
		}

	}

	

	return std::vector<int>();
	
	
}



float AStarSolver::getHeuristic(Circle* n1, int endCircle){
	Circle* end = board.indToCircle[endCircle];
	return Circle::dist(*n1,*end)/(board.maxRadius*2+board.maxDistFromNeighbour);
}
