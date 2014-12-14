#include "BoardImprover.h"
#include "CircleExtractor.h"
#include "FileReader.h"
#include "Board.h"
#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "AStarSolver.h"




void printNeigbours(std::vector<Circle*> circles, std::string imPath);
void printBoard(Board b, std::vector<int> solutionPath, int startCircle, std::vector<int> endCircles = std::vector<int>() );

int mainWrite(int argc, char** argv){


	std::vector<Circle*> circles = getCirclesFromImage(MAP_PARAMETER_SET);
	printCircles(circles,MAP_PARAMETER_SET.imagePath);

	writeBoardToFile(circles,std::string("..\\resources\\mapBoard.txt"));
	printNeigbours(circles,MAP_PARAMETER_SET.imagePath);

	return 1;

}

int mainReadCircles(int argc, char** argv){
	std::string imPath = "C:\\Users\\drorcohe\\aStarProject\\aStarProject\\heart.jpg";
	int startCircle,  endCircle;
	std::vector<Circle*> circles = readBoardFromFile(std::string("circlesFile.txt"));
	//printCircles(circles,imPath);
	printNeigbours(circles,imPath);
	return 1;
}

int main(){
	std::string imPath = SHOUT_PARAMETER_SET.imagePath;
	Board b;
	b.init(std::string("..\\resources\\shoutBoardFixed2.txt"),imPath, 5);
	
	//b.removeCircles(3);
//	HoleFillingAddCircles(b);


	BoardImprover boardImp(b);

	Circle* circle = new Circle(1,400,1,50,255,0,0);
	std::vector<Circle*> circi;
	circi.push_back(circle);
	//printCircles(circi,b.imageFilePath);
	//cv::waitKey(0);

	printCircles(b.getCircles(),b.imageFilePath);
	cv::waitKey(0);

	//thresholdBoard(b,50,50,250,1000,0,1000);
	thresholdBoard(b,5,50,250,1000,0,1000);
	HoleFillingEnlargeImages(b);
	boardImp.openGUI("..\\resources\\shoutBoardFixed2.txt");
	//thresholdBoard(b,4,50,0,1000,0,1000);
	printCircles(b.getCircles(),b.imageFilePath);

	boardImp.fixBoard();
	printCircles(b.getCircles(),b.imageFilePath);
	writeBoardToFile(b.getCircles(),"..\\resources\\shoutBoardFixed2.txt");
	cv::waitKey(0);
	//boardImp.openGUI("..\\resources\\shoutBoardFixed2.txt");
	
	
	//printBoard(b);
	printCircles(b.getCircles(),b.imageFilePath);

	writeBoardToFile(b.getCircles(),"..\\resources\\shoutBoardThreshoalded.txt");
	cv::waitKey(0);

	exit(1);
	std::vector<int> endCircles = std::vector<int>();
	endCircles.push_back(4);
	endCircles.push_back(9);
	endCircles.push_back(33);
	//endCircles.push_back(90);

	std::vector<AStarSolver::Direction> directions = std::vector<AStarSolver::Direction>();
	directions.push_back(AStarSolver::Direction::LEFT);
	directions.push_back(AStarSolver::Direction::RIGHT);
	directions.push_back(AStarSolver::Direction::RIGHT);
	//directions.push_back(AStarSolver::Direction::RIGHT);
	AStarSolver solver;



	solver.init(b,1,endCircles,directions);
	std::vector<int> solutionPath = solver.solve();
	printBoard(b,solutionPath,1,endCircles); 
	return -1;
}


void printBoard(Board b, std::vector<int> solutionPath, int startCircle, std::vector<int> endCircles ){
	cv::Mat src = cv::imread( b.imageFilePath, 1 );
	if( !src.data )
	 { exit(1); }
	cv::Mat blankMat = cv::Mat::zeros(src.size(), src.type());

	for(int i=0 ; i<b.getCircles().size() ; i++ ){
		Circle* nextCircle = b.getCircles()[i];
		cv::Point center(nextCircle->x,nextCircle->y);

		cv::Scalar scalar(255,255,255);
		if(i == startCircle){
			scalar = cv::Scalar(255,0,0);
		} else if(std::find(endCircles.begin(), endCircles.end(), i) != endCircles.end() || i==endCircles[endCircles.size()-1]){
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