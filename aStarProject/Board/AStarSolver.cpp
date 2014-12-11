#include "AStarSolver.h"
#include <set>
#include <math.h>
#include "Board.h"
#include <map>
#include <queue>
#include <functional>
#include <assert.h>
#include "FileReader.h"




std::vector<int> reconstruct_path(std::vector<int> cameFrom,int current){
	std::vector<int> path;
	while(current != -1){
		path.push_back(current);
		current = cameFrom.at(current);
	}

	return path; //TODO - reverse the order
}


std::vector<int> aStarSearch(Board& b){

	std::vector<Circle*> board = b.getCircles();
	int start = b.startCircle;
	int end = b.endCircle;

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
	f_score[start] = g_score[start] + b.getHeuristic(b.indToCircle[start]);
	
	while(openSet.size() > 0){
		
		
		int currentCircleIndex = findMinIndexFromOpenSet(openSet,f_score); //the node in openset having the lowest f_score[] value
		Circle* currentCircle = b.indToCircle[currentCircleIndex];
		if(currentCircle == b.indToCircle[b.endCircle]){
			return reconstruct_path(cameFrom, b.endCircle);
		}


		openSet.erase(currentCircle->index);
		closeSet.insert(currentCircle->index);

		for(std::vector<int>::iterator neighbour = currentCircle->neighbours.begin(); neighbour != currentCircle->neighbours.end(); ++neighbour) {
			//if the neighbour is already in the close set - continue
			if(closeSet.count(*neighbour)!=0){
				continue;
			}
			Circle* neigbourCircle = b.indToCircle[*neighbour];
			float tentative_g_score = g_score[currentCircleIndex] + 1;
			if(openSet.count(*neighbour)==0 || tentative_g_score < g_score[*neighbour]){
				cameFrom[*neighbour] = currentCircle->index;
				g_score[*neighbour] = tentative_g_score;
				f_score[*neighbour] = g_score[*neighbour] + b.getHeuristic(neigbourCircle);
				if(openSet.count(*neighbour)==0){
					openSet.insert(*neighbour);
				}
			}

		}

	}

	assert(0==1);

	return std::vector<int>();
	
	
}

