#ifndef _FILE_READER
#define _FILE_READER

#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define NEIGBOURS_HEADLINE "Neighbours"
#define BOARD_START_END_HEADLINE "BoardStartEnd"

 void writeBoardToFile(std::vector<Circle*> circles, std::string fileName, int startCircle=-1, int endCircle = -1);
 std::vector<Circle*> readBoardFromFile(std::string fileName, int& startCircle, int& endCircle);

#endif