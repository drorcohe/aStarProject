#include "BoardImprover.h"
#include "CircleExtractor.h"
#include "FileReader.h"
#include "Board.h"
#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "AStarSolver.h"
bool testSuggestions();
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

	std::string imPath = MERLIN_PARAMETER_SET.imagePath;
	Board b;
	b.init(std::string("..\\resources\\merlinBoardUpdated9.txt"),imPath);
	
	//b.removeCircles(3);
//	HoleFillingAddCircles(b);

		
	BoardImprover boardImp(b);
	printNeigboursGUI(b);
	fixColors(b);
	//thresholdBoard(b,6,50,0,1000,0,1000);
	//spaceImage(b);
	thresholdBoard(b,4,50,0,1000,0,1000);
	boardImp.openGUI("..\\resources\\merlinBoardUpdated9.txt");
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
	printNeigboursGUI(b);
	std::vector<int> endCircles = std::vector<int>();
	endCircles.push_back(49);
	endCircles.push_back(36);

	std::vector<Direction> directions = std::vector<Direction>();
	directions.push_back(Direction::LEFT);
	directions.push_back(Direction::LEFT);

	
	/*std::vector<int> endCircles = std::vector<int>();
	endCircles.push_back(4);
	endCircles.push_back(9);
	endCircles.push_back(33);
	//endCircles.push_back(90);

	std::vector<Direction> directions = std::vector<Direction>();
	directions.push_back(Direction::LEFT);
	directions.push_back(Direction::RIGHT);
	directions.push_back(Direction::RIGHT);*/
	//directions.push_back(Direction::RIGHT);
	AStarSolver solver;



	solver.init(b,38,endCircles,directions);
	std::vector<int> solutionPath = solver.solve();
	printBoard(b,solutionPath,1,endCircles); 
	return -1;
}

bool testDir(std::vector<int> solutionPath, std::vector<int> endCircles, std::vector<Direction> wantedDirections){
	bool pass = true;
	int numOfFoundTargets = 0;
	for(int i=0 ; i<solutionPath.size() ; i++){
		for(int j=0 ; j<wantedDirections.size() ;j++){
			if(solutionPath[i]==endCircles[j]){
				numOfFoundTargets++;
				if(!((wantedDirections[j]==RIGHT && i%2==0) || (wantedDirections[j]==LEFT && i%2==1))){
					pass = false;
				}
				
			}
		}
	}

	if(numOfFoundTargets!=endCircles.size())
		return false;
	else
		return pass;



}
bool testSuggestions(){
	AStarSolver solver;
	std::string imPath;
	std::vector<int> endCircles, solutionPath;
	std::vector<Direction> directions;
	Board b;
	imPath = SHOUT_PARAMETER_SET.imagePath;
	b.init(std::string("..\\resources\\finalBoard\\shoutBoard.txt"),imPath);
	endCircles = std::vector<int>();
	endCircles.push_back(4);
	endCircles.push_back(33);

	directions = std::vector<Direction>();
	directions.push_back(Direction::LEFT);
	directions.push_back(Direction::RIGHT);

	solver.init(b,1,endCircles,directions);
	solutionPath = solver.solve();

	if(!testDir(solutionPath,endCircles,directions)){
		exit(1);
	}


	endCircles = std::vector<int>();
	endCircles.push_back(2);
	endCircles.push_back(247);

	directions = std::vector<Direction>();
	directions.push_back(Direction::LEFT);
	directions.push_back(Direction::RIGHT);

	solver.init(b,10,endCircles,directions);
	solutionPath = solver.solve();
	if(!testDir(solutionPath,endCircles,directions)){
		exit(1);
	}

	endCircles = std::vector<int>();
	endCircles.push_back(356);
	endCircles.push_back(15);

	directions = std::vector<Direction>();
	directions.push_back(Direction::LEFT);
	directions.push_back(Direction::RIGHT);

	solver.init(b,442,endCircles,directions);
	solutionPath = solver.solve();
	if(!testDir(solutionPath,endCircles,directions)){
		exit(1);
	}
	b.destroy();
	




	imPath = DEER_PARAMETER_SET.imagePath;
	b.init(std::string("..\\resources\\finalBoard\\deerBoard.txt"),imPath);
	endCircles = std::vector<int>();
	endCircles.push_back(191);
	endCircles.push_back(2);

	 directions = std::vector<Direction>();
	directions.push_back(Direction::RIGHT);
	directions.push_back(Direction::LEFT);
	solver.init(b,18,endCircles,directions);
	solutionPath = solver.solve();
	if(!testDir(solutionPath,endCircles,directions)){
		exit(1);
	}


	endCircles = std::vector<int>();
	endCircles.push_back(303);
	endCircles.push_back(2);

	directions = std::vector<Direction>();
	directions.push_back(Direction::RIGHT);
	directions.push_back(Direction::LEFT);
	solver.init(b,118,endCircles,directions);
	solutionPath = solver.solve();
	if(!testDir(solutionPath,endCircles,directions)){
		exit(1);
	}
	b.destroy();

	

	imPath = HEART_PARAMETER_SET.imagePath;
	b.init(std::string("..\\resources\\finalBoard\\heartBoard.txt"),imPath);

	endCircles = std::vector<int>();
	endCircles.push_back(7);
	endCircles.push_back(2);

	directions = std::vector<Direction>();
	directions.push_back(Direction::RIGHT);
	directions.push_back(Direction::RIGHT);
	solver.init(b,98,endCircles,directions);
	solutionPath = solver.solve();
	if(!testDir(solutionPath,endCircles,directions)){
		exit(1);
	}

	endCircles = std::vector<int>();
	endCircles.push_back(49);
	endCircles.push_back(36);

	directions = std::vector<Direction>();
	directions.push_back(Direction::LEFT);
	directions.push_back(Direction::RIGHT);
	solver.init(b,38,endCircles,directions);
	solutionPath = solver.solve();
	if(!testDir(solutionPath,endCircles,directions)){
		exit(1);
	}
	b.destroy();


	printf("PASSED!");
	int a;
	std::cin>>a;



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