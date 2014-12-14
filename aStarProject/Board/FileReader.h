#ifndef _FILE_READER
#define _FILE_READER

#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define NEIGBOURS_HEADLINE "Neighbours"
#define BOARD_START_END_HEADLINE "BoardStartEnd"

 void writeBoardToFile(std::vector<Circle*> circles, std::string fileName);
void readBoardFromFile(std::string fileName,std::vector<Circle*>& circles);

#endif