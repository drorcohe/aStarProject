#ifndef _BOARD_IMPROVER
#define BOARD_IMPROVER
#include "Board.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class BoardImprover{
public:


	BoardImprover(Board& board);
	void fixBoard();
	void openGUI(std::string newFileName);
	

private:
	cv::Mat im;
	bool isNewCircleValidHere(int x, int y, int r);
	Board& b;
};


void thresholdBoard(Board& b, int min,int max, int xLeft, int xRight, int yBottom, int yTop);

#endif