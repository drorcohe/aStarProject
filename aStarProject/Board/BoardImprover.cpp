#include "BoardImprover.h"
#include "Board.h"
#include "FileReader.h"
#include "CircleExtractor.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
bool isNewCircleValidHere(std::vector<Circle*>& circles, int x, int y, int r, std::set<Circle*> ignoreList= std::set<Circle*>(), bool printRes = false);
char latestPressedKey = -1;
std::vector<Circle*>* gCircles;  
cv::Mat gMat, gIm;
int gR=125; int gB=125; int gG = 125;
BoardImprover::BoardImprover(Board &board) : b(board){
	im = cv::imread( board.imageFilePath, 1 );

}

bool isNewCircleValidHere(std::vector<Circle*>& circles, int x, int y, int r, std::set<Circle*> ignoreList, bool printRes){
	std::vector<Circle*>::iterator it;
	for (it =circles.begin(); it != circles.end(); it++) {
		if(ignoreList.find(*it) != ignoreList.end()){
			continue;
		}
		if(doesCirclesIntersect(x,y,r,(*it)->x,(*it)->y,(*it)->radius)){
			if(printRes){
				printf("x:%d,y:%d,r:%d intersect with x:%d,y:%d,r:%d ",x,y,r,(*it)->x,(*it)->y,(*it)->radius);
			}
			return false;
		}
	}
	return true;
}

bool addCircle(std::vector<Circle*>& circles, int x, int y, int r, int R=0, int G=0, int B=0){
	if(! isNewCircleValidHere(circles, x,y,r) ){
		return false;
	}
	else{
		circles.push_back(new Circle(x,y,-1,r,R,G,B));
	}
}



void removeCircle(std::vector<Circle*>& circles, Circle* circle){
	std::vector<Circle*>::iterator it;
    for (it = circles.begin(); it != circles.end(); /* DONT increment here*/) {
		if(*it == circle){
			it = circles.erase(it);
		}else
        {
            it++;
        }
    }


}

cv::Mat reRenderCircles(std::vector<Circle*> circles, cv::Size size, int type){

	cv::Mat circlesMat = cv::Mat::zeros(size,type);

	for(int i=0; i<circles.size() ; i++ ){
		Circle* nextCircle = circles[i];
		cv::Point center(nextCircle->x,nextCircle->y);
		int R = nextCircle->R; int G = nextCircle->G; int B = nextCircle->B;
		circle( circlesMat, center, nextCircle->radius, cv::Scalar(R,G,B),CV_FILLED );
	}

	std::cout<<"renderd " <<circles.size()<<" circles \n";
	cv::imshow( "circles rendering", circlesMat );

	return circlesMat;
}

Circle* findClosestCircle(std::vector<Circle*> circles, int x, int y){
	int closestSoFar = INT_MAX;
	Circle* closestCircle = NULL;

	for(int i=0 ; i < circles.size() ; i++){
		Circle* c = circles[i];
		float dist = std::sqrt(std::pow(c->x-x,2)+std::pow(c->y-y,2)) - c->radius;
		if(dist < closestSoFar){
			closestSoFar = dist;
			closestCircle = c;
		}

	}


	return closestCircle;

}

#define DEFAULT_RADIUS 11
static void onMouse( int event, int x, int y, int, void* )
{
    if( event != cv::EVENT_LBUTTONDOWN )
        return;

	if(latestPressedKey=='a'){
		int radius = DEFAULT_RADIUS;

		while(!isNewCircleValidHere(*gCircles,x,y,radius) && radius > 4){
			radius--;
		}

		if (isNewCircleValidHere(*gCircles,x,y,radius,std::set<Circle*>(), true) ){
			int R = gIm.at<cv::Vec3b>(y,x)[0]; int G = gIm.at<cv::Vec3b>(y,x)[1]; int B = gIm.at<cv::Vec3b>(y,x)[2];
			addCircle(*gCircles,x,y,radius,R,G,B);

		}else{
			std::cout<<"new Circle isn't valid here"<<std::endl;
		}
	}else{
		Circle* closestCircle = findClosestCircle(*gCircles,x,y);
		if(latestPressedKey=='i'|| latestPressedKey=='j'){
			int addition = latestPressedKey=='i'? 1:-1;
			std::set<Circle*> ignoreSet = std::set<Circle*>();
			ignoreSet.insert(closestCircle);
			if (!isNewCircleValidHere(*gCircles,x,y,closestCircle->radius+addition,ignoreSet,true) ){
				std::cout<<"cant increment radius, it is too big"<<std::endl;
			}else{
				closestCircle->radius += addition;
			}			
		} else if(latestPressedKey=='d'){
			removeCircle(*gCircles,closestCircle);
			std::cout<<"removed circle: x: "<<closestCircle->x <<" y: "<<closestCircle->y <<" with radius " <<  closestCircle->radius << std::endl;
		}else if(latestPressedKey=='c'){
			gR = closestCircle->R;
			gG = closestCircle->G;
			gB = closestCircle->B;
			printf("changed chosen color to: %d,%d,%d",gR,gG,gB);
			
		} else if(latestPressedKey=='v'){
			closestCircle->R = gR;
			closestCircle->G = gG;
			closestCircle->B = gB;
			printf("changed chosen color to: %d,%d,%d",gR,gG,gB);
			
		}else if((latestPressedKey=='o' || latestPressedKey=='l' || latestPressedKey=='k' || latestPressedKey==';' )){
			std::set<Circle*> ignoreSet = std::set<Circle*>();
			ignoreSet.insert(closestCircle);

			int xAdd=0; int yAdd = 0;
			if(latestPressedKey=='k') //left
				xAdd--;
			else if(latestPressedKey=='o') //up
				yAdd--;
			else if(latestPressedKey==';') //right 
				xAdd++;
			else if(latestPressedKey=='l') //down 
				yAdd++;

			if (isNewCircleValidHere(*gCircles,closestCircle->x,closestCircle->y,closestCircle->radius,ignoreSet, true) ){
				int R = gIm.at<cv::Vec3b>(y,x)[0]; int G = gIm.at<cv::Vec3b>(y,x)[1]; int B = gIm.at<cv::Vec3b>(y,x)[2];
				closestCircle->x += xAdd;
				closestCircle->y += yAdd;
			}else{
				std::cout<<"cand change circle location, it's not valid here"<<std::endl;
			}
		}

	}

	cv::destroyAllWindows();
	gMat = reRenderCircles(*gCircles,gIm.size(),gIm.type());
	cv::setMouseCallback(std::string("circles rendering"), onMouse);

	
    
}



void BoardImprover::openGUI(std::string newFileName){
	gCircles = &b.getCirclesRef();
	//*gCircles = b.getCirclesRef();
	gIm = im.clone();
	gMat = reRenderCircles(*gCircles,gIm.size(),gIm.type());
	cv::setMouseCallback(std::string("circles rendering"), onMouse);
	std::cout<<"press ESC to exit \n d to delete circle \n a to add circle \n i to increment radius \n j to decrement radius \n r to render again here \n s  to save\n  h for hole filling \n( o k l ;) for changing direction \nc for choose color \nv for applying color \n";

	while(1==1){
		char newKey = cv::waitKey();
		if(latestPressedKey != newKey){
			if(newKey=='d' || newKey=='a' || newKey=='i' || newKey=='j' || newKey=='s' || newKey=='r'|| newKey==27 || (newKey=='o' || newKey=='l' || newKey=='k' || newKey==';'  || newKey=='v' || newKey=='c' )){
				std::cout<<"switched to operation: "<< std::string(1,newKey)<<std::endl;
				latestPressedKey = newKey;
			}
			if(newKey == 's'){
				fixBoard();
				writeBoardToFile(b.getCirclesRef(),newFileName);
				std::cout<<"board was written to file\n";
			}else if(newKey == 'h'){
				HoleFillingEnlargeImages(b);
				gMat = reRenderCircles(*gCircles,gIm.size(),gIm.type());
				cv::setMouseCallback(std::string("circles rendering"), onMouse);
				std::cout<<"hole filling was performed\n";
			}else if(newKey==27){
				exit(1);
			}else if(newKey=='r'){
				cv::destroyAllWindows();
				gMat = reRenderCircles(*gCircles,gIm.size(),gIm.type());
				cv::setMouseCallback(std::string("circles rendering"), onMouse);
				std::cout<<"rerendered screen\n";
			}

		}
	}
}


void BoardImprover::fixBoard(){

	for(int i=0 ; i<b.getCirclesRef().size() ; i++){
		b.getCirclesRef()[i]->index = i;
		b.getCirclesRef()[i]->neighbours = std::vector<int>();
	}

	extractNeigbours(b.getCirclesRef(),b.maxDistFromNeighbour,false);

}


void thresholdBoard(Board& b, int min,int max, int xLeft, int xRight, int yBottom, int yTop){
	std::vector<Circle*>::iterator it;
	for (it = b.getCirclesRef().begin(); it != b.getCirclesRef().end(); /* DONT increment here*/) {
		Circle* c = *it;
		if(((*it)->x >= xLeft && (*it)->x <= xRight && (*it)->y >= yBottom && (*it)->y <= yTop) && ((*it)->radius <=min || (*it)->radius >= max)){
			Circle* c = *it;
			it = b.getCirclesRef().erase(it);
		}else
        {
			it++;

        }
    }

}



void HoleFillingEnlargeImages(Board& board){

	std::vector<Circle*> circles = board.getCircles();
	int nextIndex = circles.size() + 1;
	float addFactor = 1;

	for(int k=0 ; k<15 ; k++){

		for(int i=0 ; i<circles.size() ; i++){
			Circle* circle = circles[i];
			int newRadius = circle->radius + addFactor;

			bool changeRadius = true;
			for(int j=0 ; j<circles.size() ; j++){
				Circle* secondCircle =circles[j];
				if(i==j){
					continue;
				}
				if(doesCirclesIntersect(circle->x,circle->y,newRadius,secondCircle->x,secondCircle->y,secondCircle->radius)){
					changeRadius = false;
					break;
				}
			}
			if(changeRadius){
				circle->radius = newRadius;
			}
			
		}
	}
}



#define MIN_RADIUS 2
void HoleFillingAddCircles(Board& board){




	std::vector<Circle*>& circles = board.getCirclesRef();
	/*
	for(int i=0 ; i<circles.size() ; i++){
		circles[i]->neighbours = vector<int>();
	}
	extractNeigbours(circles, board.maxDistFromNeighbour*, false);
	*/
	
	int nextIndex = circles.size();
	for(int i=0 ; i<circles.size() ; i++){
		Circle* firstCircle = circles[i];
		for(int j=0 ; j<firstCircle->neighbours.size() ; j++){
			
			Circle* secondCircle =circles[firstCircle->neighbours[j]];

			float newCircleX = (secondCircle->x + firstCircle->x) / 2;
			float newCircleY = (secondCircle->y + firstCircle->y) / 2;
			float radius = std::max(firstCircle->radius,secondCircle->radius);

			while(radius >= MIN_RADIUS){
				bool addNewCircle = true;
				for(int k=0 ; k<circles.size() ; k++){
					if(doesCirclesIntersect(newCircleX,newCircleY,radius,circles[k]->x,circles[k]->y,circles[k]->radius)){
						addNewCircle = false;
						break;
					}
				}
				if(addNewCircle){
					int R = (firstCircle->R + secondCircle->R)/2;
					int G = (firstCircle->G + secondCircle->G)/2;
					int B = (firstCircle->B + secondCircle->B)/2;

					Circle* newCircle = new Circle(newCircleX,newCircleY,nextIndex,radius,R,G,B);
					circles.push_back(newCircle);
					nextIndex++;
				}else{
					radius = radius / 1.2;
					break;
				}
				
	
			}
			
		}
	}

	for(int i=0 ; i<circles.size() ; i++){
		circles[i]->neighbours = std::vector<int>();
	}

	board.maxDistFromNeighbour = board.maxDistFromNeighbour / 2;
	extractNeigbours(circles, board.maxDistFromNeighbour);

	int a = 3;
}


void spaceImage(Board& board){
	for(int i=0 ; i<board.getCircles().size() ; i++){
		Circle* nextCircle = board.getCircles()[i];
		nextCircle->radius = nextCircle->radius*1.4;
		nextCircle->x = nextCircle->x*1.4;
		nextCircle->y = nextCircle->y*1.4;
	}

}

void fixColors(Board& board){
	for(int i=0 ; i<board.getCircles().size() ; i++){
		Circle* nextCircle = board.getCircles()[i];
		if(nextCircle->R + nextCircle->G + nextCircle->B < 100){
			int newR=0;int newG=0;int newB=0;
			for(int j=0 ; j<nextCircle->neighbours.size() ; j++){
				Circle* neigbour = board.indToCircle[nextCircle->neighbours[j]];
				newR = newR + neigbour->R;
				newG = newG + neigbour->G;
				newB = newB + neigbour->B;
			}
			nextCircle->R = newR / nextCircle->neighbours.size();
			nextCircle->G = newG / nextCircle->neighbours.size();
			nextCircle->B = newB / nextCircle->neighbours.size();
			if(abs(nextCircle->R - nextCircle->G) < 30 && + abs(nextCircle->R - nextCircle->G) < 30 && abs(nextCircle->R - nextCircle->G) < 30){
				nextCircle->B = nextCircle->B + 100 % 255;
			}
		}
		
	}

}