#include "BoardImprover.h"
#include "CircleExtractor.h"
#include "FileReader.h"
#include "Board.h"
#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "AStarSolver.h"

void printNeigboursGUI(Board & b);


void printNeigbours(std::vector<Circle*> circles, std::string imPath, Circle* chosenCircle);
void printBoard(Board b, std::vector<int> solutionPath, int startCircle, std::vector<int> endCircles = std::vector<int>() );

int mainWrite(int argc, char** argv){


	std::vector<Circle*> circles = getCirclesFromImage(MAP_PARAMETER_SET);
	printCircles(circles,MAP_PARAMETER_SET.imagePath);

	writeBoardToFile(circles,std::string("..\\resources\\mapBoard.txt"));
	//printNeigbours(circles,MAP_PARAMETER_SET.imagePath);

	return 1;

}

int mainNe(int argc, char** argv){
	std::string imPath = "C:\\Users\\drorcohe\\aStarProject\\aStarProject\\resources\\shout.jpg";
	int startCircle,  endCircle;
	std::vector<Circle*> circles;
	readBoardFromFile(std::string("..\\resources\\shoutBoardFixed4.txt"),circles);
	//printCircles(circles,imPath);
	//printNeigbours(circles,imPath);
	return 1;
}

int main(){
	std::string imPath = SHOUT_PARAMETER_SET.imagePath;
	Board b;
	b.init(std::string("..\\resources\\shoutBoardFixedLatest.txt"),imPath, 3.5);
	
	//b.removeCircles(3);
//	HoleFillingAddCircles(b);

	
	BoardImprover boardImp(b);
	//printNeigboursGUI(b);
	//boardImp.fixBoard();
	//writeBoardToFile(b.getCircles(),"..\\resources\\shoutBoardFixedLatest.txt");
	//fixColors(b);
	//spaceImage(b);
	//thresholdBoard(b,3,50,0,1000,0,1000);
	//boardImp.openGUI("..\\resources\\shoutBoardFixed6.txt");
	//boardImp.fixBoard();

	//writeBoardToFile(b.getCircles(),"..\\resources\\shoutBoardFixed3.txt");
	//printCircles(circi,b.imageFilePath);
	//cv::waitKey(0);

	//printCircles(b.getCircles(),b.imageFilePath);
	//cv::waitKey(0);
	/*cv::waitKey(0);

	HoleFillingEnlargeImages(b);
	thresholdBoard(b,3,50,0,1000,0,1000);
	thresholdBoard(b,5,50,250,1000,0,1000);
	//HoleFillingEnlargeImages(b);
	boardImp.openGUI("..\\resources\\shoutBoardFixed3.txt");
	//thresholdBoard(b,4,50,0,1000,0,1000);
	printCircles(b.getCircles(),b.imageFilePath);

	//boardImp.fixBoard();
	printCircles(b.getCircles(),b.imageFilePath);
	//writeBoardToFile(b.getCircles(),"..\\resources\\shoutBoardFixed2.txt");
	cv::waitKey(0);
	//boardImp.openGUI("..\\resources\\shoutBoardFixed2.txt");
	
	
	//printBoard(b);
	printCircles(b.getCircles(),b.imageFilePath);

	writeBoardToFile(b.getCircles(),"..\\resources\\shoutBoardThreshoalded.txt");
	cv::waitKey(0);

	exit(1);*/
	//cv::waitKey();

	std::vector<int> endCircles = std::vector<int>();
	//endCircles.push_back(13);
	endCircles.push_back(37);
	//endCircles.push_back(409);


	std::vector<AStarSolver::Direction> directions = std::vector<AStarSolver::Direction>();
	//directions.push_back(AStarSolver::Direction::LEFT);
	directions.push_back(AStarSolver::Direction::RIGHT);
	//directions.push_back(AStarSolver::Direction::RIGHT	);

	/*std::vector<int> endCircles = std::vector<int>();
	endCircles.push_back(4);
	endCircles.push_back(9);
	endCircles.push_back(33);
	//endCircles.push_back(90);

	std::vector<AStarSolver::Direction> directions = std::vector<AStarSolver::Direction>();
	directions.push_back(AStarSolver::Direction::LEFT);
	directions.push_back(AStarSolver::Direction::RIGHT);
	directions.push_back(AStarSolver::Direction::RIGHT);*/
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
		cv::circle( blankMat, center, nextCircle->radius, scalar );

	}
	for(int i=0 ; i<solutionPath.size() ; i++ ){
		Circle* nextCircle = b.indToCircle[solutionPath[i]];
		cv::Scalar scalar(0,0,255);
		if(solutionPath[i] == startCircle){
			scalar = cv::Scalar(255,0,0);
		} else if(std::find(endCircles.begin(), endCircles.end(), solutionPath[i]) != endCircles.end() || solutionPath[i]==endCircles[endCircles.size()-1]){
			scalar = cv::Scalar(0,255,0);
		}
		cv::Point center(nextCircle->x,nextCircle->y);
		cv::circle( blankMat, center, nextCircle->radius, scalar );
		printf("next circle: %d, direction: %c \n", nextCircle->index, (i%2==0? 'R':'L'));
		

	}

	cv::imshow( "neighbours Demo", blankMat );
	cv::waitKey(0.5);
}

std::vector<Circle*> gCircles;
std::string gImPath;
static void onMouse( int event, int x, int y, int, void* )
	{
    if( event != cv::EVENT_LBUTTONDOWN )
        return;

	Circle* closestCircle = findClosestCircle(gCircles,x,y);
	printNeigbours(gCircles,gImPath,closestCircle);
	cv::setMouseCallback(std::string("neighbours Demo"), onMouse);
}


void printNeigboursGUI(Board & b){

	gImPath = std::string(b.imageFilePath);
	gCircles = b.getCircles();
	printNeigbours(gCircles, b.imageFilePath,NULL);
	cv::setMouseCallback(std::string("neighbours Demo"), onMouse);
	cv::waitKey();
	
}

void printNeigbours(std::vector<Circle*> circles, std::string imPath, Circle* chosenCircle){

	cv::Mat src = cv::imread( imPath, 1 );
	if( !src.data )
	 { exit(1); }
	
		
	//prints all the other circles
	cv::Mat blankMat = cv::Mat::zeros(src.size(), src.type());
	for(int i=0 ; i<circles.size() ; i++ ){
		Circle* nextCircle = circles[i];
		cv::Point center(nextCircle->x,nextCircle->y);
		circle( blankMat, center, circles[i]->radius, cv::Scalar(255,255,255) );

	}

	if(!chosenCircle == NULL){
		printf("chosen circle: %d, location: x=%d, y=%d, rad=%d\n", chosenCircle->index, chosenCircle->x, chosenCircle->y, chosenCircle->radius);
		//prints the chosen circle
		cv::Point center(chosenCircle->x,chosenCircle->y);
		circle( blankMat, center, chosenCircle->radius, cv::Scalar(255,0,0) );

		//prints its neighours
		for(int i=0 ; i<chosenCircle->neighbours.size() ; i++){

			Circle* neighbourCircle = circles[chosenCircle->neighbours[i]];
			cv::Point center(neighbourCircle->x,neighbourCircle->y);
			circle( blankMat, center, neighbourCircle->radius, cv::Scalar(0,255,0) );
			printf("chosen neigbour: %d, location: x=%d, y=%d, rad=%d\n", neighbourCircle->index, neighbourCircle->x, neighbourCircle->y, neighbourCircle->radius);
		}
	}
			
	

	cv::imshow( "neighbours Demo", blankMat );

			
	

}