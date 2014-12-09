#include "CircleExtractor.h"
#include "FileReader.h"
#include "Board.h"
#include <iostream>
#include <string>

#define RESOURCES_DIR "..\\resources\\"


int main(){
	Board b;
	b.init(std::string("..\\resources\\circlesFile.txt"),std::string("..\\resources\\heart.jpg") );


	std::vector<int> solutionPath = b.aStarSearch();

	return -1;
}






