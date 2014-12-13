#include "BoardImprover.h"
#include "Board.h"
#include "FileReader.h"
#include "CircleExtractor.h"
#include <iostream>
#include <string>
#include <vector>


char latestPressedKey = -1;
std::vector<Circle*>* gCircles;  
cv::Mat gMat, gIm;

BoardImprover::BoardImprover(Board &board) : b(board){
	im = cv::imread( board.imageFilePath, 1 );

}

bool isNewCircleValidHere(std::vector<Circle*>& circles, int x, int y, int r){
	std::vector<Circle*>::iterator it;
	for (it =circles.begin(); it != circles.end(); it++) {
		if(doesCirclesIntersect(x,y,r,(*it)->x,(*it)->y,(*it)->radius)){
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
		cv::Point center(nextCircle->y,nextCircle->x);
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

#define DEFAULT_RADIUS 7
static void onMouse( int event, int x, int y, int, void* )
{
    if( event != cv::EVENT_LBUTTONDOWN )
        return;

	
	if(latestPressedKey=='a'){
		int radius = DEFAULT_RADIUS;
		while(!isNewCircleValidHere(*gCircles,x,y,radius) && radius > 1){
			radius--;
		}
		if (isNewCircleValidHere(*gCircles,x,y,radius) ){
			int R = gIm.at<cv::Vec3b>(y,x)[0]; int G = gIm.at<cv::Vec3b>(y,x)[1]; int B = gIm.at<cv::Vec3b>(y,x)[2];
			addCircle(*gCircles,x,y,radius,255,0,0);

		}else{
			std::cout<<"new Circle isn't valid here"<<std::endl;
		}
	}else{
		Circle* closestCircle = findClosestCircle(*gCircles,x,y);
		if(latestPressedKey=='i'|| latestPressedKey=='j'){
			int addition = latestPressedKey=='i'? 1:-1;
			if (!isNewCircleValidHere(*gCircles,x,y,closestCircle->radius+addition) ){
				std::cout<<"cant increment radius, it is too big"<<std::endl;
			}else{
				closestCircle->radius += addition;
			}			
		} else if(latestPressedKey=='d'){
			removeCircle(*gCircles,closestCircle);
			std::cout<<"removed circle: x: "<<closestCircle->x <<" y: "<<closestCircle->y <<" with radius " <<  closestCircle->radius << std::endl;
		}

	}

	
    
}



void BoardImprover::openGUI(std::string newFileName){
	gCircles = &b.getCirclesRef();
	//*gCircles = b.getCirclesRef();
	gIm = im.clone();
	gMat = reRenderCircles(*gCircles,gIm.size(),gIm.type());
	cv::setMouseCallback(std::string("circles rendering"), onMouse);
	std::cout<<"press ESC to exit \n d to delete circle \n a to add circle \n i to increment radius \n j to decrement radius \n r to render again here \n s  to save\n";

	while(1==1){
		char newKey = cv::waitKey();
		if(latestPressedKey != newKey){
			if(newKey=='d' || newKey=='a' || newKey=='i' || newKey=='j' || newKey=='s' || newKey=='r'|| newKey==27){
				std::cout<<"moved to operation: "<< std::string(1,newKey)<<std::endl;
				latestPressedKey = newKey;
			}
			if(newKey == 's'){
				fixBoard();
				writeBoardToFile(b.getCirclesRef(),newFileName);
			}else if(newKey==27){
				exit(1);
			}else if(newKey=='r'){
				cv::destroyAllWindows();
				gMat = reRenderCircles(*gCircles,gIm.size(),gIm.type());
			}

		}
	}
}


void BoardImprover::fixBoard(){

	for(int i=0 ; i<b.getCirclesRef().size() ; i++){
		b.getCirclesRef()[i]->index = i;
		b.getCirclesRef()[i]->neighbours = std::vector<int>();
	}
	extractNeigbours(b.getCirclesRef());

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
