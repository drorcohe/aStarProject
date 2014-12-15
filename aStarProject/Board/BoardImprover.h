#ifndef _BOARD_IMPROVER
#define BOARD_IMPROVER
#include "Board.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <set>

class BoardImprover{
public:


	BoardImprover(Board& board);
	void fixBoard();
	void openGUI(std::string newFileName);
	

private:
	cv::Mat im;
	//bool isNewCircleValidHere(std::vector<Circle*>& circles, int x, int y, int r, std::set<Circle*> ignoreList= std::set<Circle*>());
	Board& b;
};


void thresholdBoard(Board& b, int min,int max, int xLeft, int xRight, int yBottom, int yTop);

void HoleFillingEnlargeImages(Board& board);

void HoleFillingAddCircles(Board& board);

void fixColors(Board& board);

void spaceImage(Board& board);

#endif