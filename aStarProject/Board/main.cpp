#include "CircleExtractor.h"
#include "FileReader.h"
#include "Board.h"
#include <iostream>
#include <string>



int main(){
	std::string imPath = "C:\\Users\\drorcohe\\aStarProject\\aStarProject\\heart.jpg";
	Board b;
	b.init(std::string("circlesFile.txt"), imPath);


	std::vector<int> solutionPath = b.aStarSearch();

	return -1;
}






