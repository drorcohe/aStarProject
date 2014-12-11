#include "CircleExtractor.h"
#include "FileReader.h"
#include "Board.h"
#include <iostream>
#include <string>
#include "AStarSolver.h"
#define RESOURCES_DIR "..\\resources\\"


int main(){


	Board b;
	b.init(std::string("..\\resources\\shoutBoard.txt"),std::string("..\\resources\\shout.jpg"), 5);

	//sets all the end circles (circles which we want to touch)
	std::vector<int> endCircles = std::vector<int>();
	endCircles.push_back(4);
	endCircles.push_back(9);
	endCircles.push_back(33);

	//sets the end circles directions
	std::vector<AStarSolver::Direction> directions = std::vector<AStarSolver::Direction>();
	directions.push_back(AStarSolver::Direction::LEFT);
	directions.push_back(AStarSolver::Direction::RIGHT);
	directions.push_back(AStarSolver::Direction::RIGHT);
	AStarSolver solver;



	solver.init(b,1,endCircles,directions);
	std::vector<int> solutionPath = solver.solve();

	return -1;

}






