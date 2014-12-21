#ifndef _FILE_READER
#define _FILE_READER

#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define NEIGBOURS_HEADLINE "Neighbours"
#define BOARD_START_END_HEADLINE "BoardStartEnd"
#define RECOMMENDED_PATHS_HEADLINE "RecommendedStartEnd"

 void writeBoardToFile(std::vector<Circle*> circles, int w, int h, std::vector<PATH_DETAIL> paths,  std::string fileName);
void readBoardFromFile(std::string fileName,std::vector<Circle*>& circles,int& w, int& h, std::vector<PATH_DETAIL>& paths);

#endif