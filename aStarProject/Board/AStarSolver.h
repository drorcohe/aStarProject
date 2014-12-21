#ifndef _ASTAR_SOLVER
#define _ASTAR_SOLVER
#define DEBUG
#include "Board.h"
#include "SharedTypes.h"




typedef struct AStarNode{
	int circleIndex;
	Direction dir;

	bool operator <(const AStarNode& other) const{
		if(other.circleIndex == circleIndex){
			return other.dir<dir;
		}else{
			return other.circleIndex < circleIndex;
		}
	}

}AStarNode;

class AStarSolver{
public:
		
	void init(Board& board, int startCircle, int endCircle, Direction direction = NOT_IMPORTANT);
	void init(Board& board, int startCircle, std::vector<int> endCircles, std::vector<Direction> directions);
	std::vector<int> solve();


	
	std::vector<int> endCircles;
	std::vector<Direction> targetSpinDirections;
	int startCircle;

private:

	std::vector<int> aStarSearch(int start,int end, Direction directionStart,Direction directionEnd, std::vector<AStarNode> visitedNodes=std::vector<AStarNode>() );
	float getHeuristic(Circle* n1, int endCircle);
	Board board;

};







#endif