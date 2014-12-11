#ifndef _ASTAR_SOLVER
#define _ASTAR_SOLVER
#define DEBUG
#include "Board.h"

class AStarSolver{
public:
		
	typedef enum Direction{
		RIGHT, LEFT, NOT_IMPORTANT
	}Direction;

	typedef enum PathType{
		EVEN, ODD, TYPE_NOT_IMPORTANT
	}PathType;

	void init(Board& board, int startCircle, int endCircle, AStarSolver::Direction direction = NOT_IMPORTANT);
	void init(Board& board, int startCircle, std::vector<int> endCircles, std::vector<AStarSolver::Direction> directions);
	std::vector<int> solve();


	
	std::vector<int> endCircles;
	std::vector<AStarSolver::Direction> targetSpinDirections;
	int startCircle;

private:
	std::vector<int> aStarSearch(int start,int end,AStarSolver::PathType pathLength = AStarSolver::PathType::TYPE_NOT_IMPORTANT, std::vector<int> invalidIndices=std::vector<int>());
	float getHeuristic(Circle* n1, int endCircle);
	Board board;

};








#endif