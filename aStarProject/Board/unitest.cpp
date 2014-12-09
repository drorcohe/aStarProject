#include "CircleExtractor.h"
#include "FileReader.h"
#include "Board.h"
#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void printNeigbours(std::vector<Circle*> circles, std::string imPath);
void printBoard(Board b, std::vector<int> solutionPath = std::vector<int>());

int main(int argc, char** argv){


	std::vector<Circle*> circles = getCirclesFromImage(DEER_PARAMETER_SET);
	//printCircles(circles,imPath);

	writeBoardToFile(circles,std::string("..\\resources\\deerBoard.txt"),4,199);
	printNeigbours(circles,DEER_PARAMETER_SET.imagePath);

	return 1;

}

int mainReadCircles(int argc, char** argv){
	std::string imPath = "C:\\Users\\drorcohe\\aStarProject\\aStarProject\\heart.jpg";
	int startCircle,  endCircle;
	std::vector<Circle*> circles = readBoardFromFile(std::string("circlesFile.txt"),startCircle,endCircle);
	//printCircles(circles,imPath);
	printNeigbours(circles,imPath);
	return 1;
}

int mainPrintSolution(){
	std::string imPath = "C:\\Users\\drorcohe\\aStarProject\\aStarProject\\heart.jpg";
	Board b;
	b.init(std::string("circlesFile.txt"),imPath);
	printBoard(b);

	std::vector<int> solutionPath = b.aStarSearch();
	printBoard(b,solutionPath); 
	return -1;
}


void printBoard(Board b, std::vector<int> solutionPath){
	cv::Mat src = cv::imread( b.imageFilePath, 1 );
	if( !src.data )
	 { exit(1); }
	cv::Mat blankMat = cv::Mat::zeros(src.size(), src.type());

	for(int i=0 ; i<b.getCircles().size() ; i++ ){
		Circle* nextCircle = b.getCircles()[i];
		cv::Point center(nextCircle->y,nextCircle->x);

		cv::Scalar scalar(255,255,255);
		if(i == b.startCircle){
			scalar = cv::Scalar(255,0,0);
		} else if(i == b.endCircle){
			scalar = cv::Scalar(0,255,0);
		}else{
			for(int j=0 ; j<solutionPath.size() ; j++){
				if(solutionPath[j]==i){
					scalar = cv::Scalar(0,0,255);
				}
			}
		}

		cv::circle( blankMat, center, nextCircle->radius, scalar );

	}

	cv::imshow( "neighbours Demo", blankMat );
	cv::waitKey(0.5);
}


void printNeigbours(std::vector<Circle*> circles, std::string imPath){

	cv::Mat src = cv::imread( imPath, 1 );
	if( !src.data )
	 { exit(1); }

	cv::Mat blankMat;

	int maxIndex = -1;
	int minIndex = 1000000;



	for(int i=0 ; i< circles.size() ; i++){
		minIndex = std::min(minIndex,circles[i]->index);
		maxIndex = std::max(maxIndex,circles[i]->index);
	}

	std::cout<<"min index is "<<minIndex<<", max index is: " << maxIndex <<std::endl;
	
	while(0==0){
		std::cout<<"choose index, or press any other key to exit"<<std::endl;
		int nextIndex;
		std::cin>>nextIndex;
		if(nextIndex >= minIndex && minIndex <= maxIndex){
			cv::destroyAllWindows();
			//prints all the other circles
			cv::Mat blankMat = cv::Mat::zeros(src.size(), src.type());
			for(int i=0 ; i<circles.size() ; i++ ){
				Circle* nextCircle = circles[i];
				cv::Point center(nextCircle->y,nextCircle->x);
				circle( blankMat, center, circles[i]->radius, cv::Scalar(255,255,255) );

			}

			
			//prints the chosen circle
			Circle* centerCircle = circles[nextIndex];
			cv::Point center(centerCircle->y,centerCircle->x);
			circle( blankMat, center, centerCircle->radius, cv::Scalar(255,0,0) );

			//prints its neighours
			for(int i=0 ; i<centerCircle->neighbours.size() ; i++){

				Circle* neighbourCircle = circles[centerCircle->neighbours[i]];
				cv::Point center(neighbourCircle->y,neighbourCircle->x);
				circle( blankMat, center, neighbourCircle->radius, cv::Scalar(0,255,0) );

			}

			cv::imshow( "neighbours Demo", blankMat );
			cv::waitKey(0.5);
			
		}
		else{
			break;
		}
	}


}