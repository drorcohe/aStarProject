#ifndef _FILE_READER
#define _FILE_READER

#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>

 void writeCirclesToFile(std::vector<Circle*> circles, std::string fileName);
 std::vector<Circle*> readCirclesFromFile(std::string fileName);

#endif