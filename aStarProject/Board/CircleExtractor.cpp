#include "CircleExtractor.h"
#include "Board.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include "FileReader.h"

using namespace cv;
bool doesCirclesIntersect(float x0, float y0, float r0, float x1, float y1, float r1);
extern ParametersSet HEART_PARAMETER_SET = {std::string("HEART"),std::string("..\\resources\\heart.jpg"),1, 6 ,2,13,2,17,15};
extern ParametersSet DEER_PARAMETER_SET = {std::string("HEART"),std::string("..\\resources\\deer.jpg"),1, 6 ,2,7,2,17,10};

//first image: C:\\Users\\drorcohe\\aStarProject\\aStarProject\\heart.jpg
//image parameters: HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 6 ,2,13,2,17); 

/*
	CV_HOUGH_GRADIENT: Define the detection method. Currently this is the only one available in OpenCV
	dp = 1: The inverse ratio of resolution
	min_dist = src_gray.rows/8: Minimum distance between detected centers
	param_1 = 200: Upper threshold for the internal Canny edge detector
	param_2 = 100*: Threshold for center detection.
	min_radius = 0: Minimum radio to be detected. If unknown, put zero as default.
	max_radius = 0: Maximum radius to be detected. If unknown, put zero as default
*/



/** @function main */
int main3(int argc, char** argv)
{
  Mat src, src_orig, src_gray, out;

  /// Read the image
  src_orig = imread( "C:\\Users\\drorcohe\\aStarProject\\aStarProject\\heart.jpg", 1 );

  if( !src_orig.data )
    { return -1; }
  //out = Mat::zeros(src_orig.size(), src_orig.type());

  src = src_orig.clone();
  /// Convert it to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  cv::Mat thresh;
  //cv::threshold(src_gray,src_gray,100,255,src_gray.type()); monro

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;
  //1, 2 ,2,100,1
  //HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 2 ,2,25,1,30); // min and max radius
 // HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 2 ,2,11,2,11); monro
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 6 ,2,13,2,17); // heart
  /// Apply the Hough Transform to find the circles
  //HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
   }

  /// Show your results
  namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  imshow( "Hough Circle Transform Demo", src );

  waitKey(0);
  return 0;
}



std::vector<Circle*> getCirclesFromImage(ParametersSet set){
	return getCirclesFromImage(set.imagePath, set.dp,set.minDist,set.param1,set.param2,set.minRadius,set.maxRadius,set.maxDistFromNeighbour);
}

std::vector<Circle*> getCirclesFromImage(std::string path, 
			double dp, double minDist, double param1, double param2, int minRadius, int maxRadius,float maxDistanceFromNeighbours){

	Mat src, src_orig, src_gray, out;

  /// Read the image
  src = imread( path, 1 );

  if( !src.data )
    { exit(1); }

  /// Convert it to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );


  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;
  std::vector<Circle*> outCircles;
  //1, 2 ,2,100,1

    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius); 
 
  /// Draw the circles detected
	int circleCounter = 0;
  for( size_t i = 0; i < circles.size(); i++ )
  {
	  
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
	  Circle* next = new Circle(center.y,center.x,circleCounter,radius,src.at<cv::Vec3b>(center.y,center.x)[0],src.at<cv::Vec3b>(center.y,center.x)[1],src.at<cv::Vec3b>(center.y,center.x)[2]);
      
	  //checks if the current circle collides with previos ones
		bool isCollided = false;
		for( size_t j = 0; j < circleCounter; j++ )
		{
			Circle* prevCircle = outCircles[j];
			if(Circle::dist(*prevCircle,*next) < -0.5){
				isCollided = true;
				break;
			}
		}

		if(isCollided){
			delete next;
			continue;
		}

		circleCounter++;
		outCircles.push_back(next);
   }

	extractNeigbours(outCircles,maxDistanceFromNeighbours);
	return outCircles;


}


int main2(){
	std::vector<Circle*> circles = getCirclesFromImage(DEER_PARAMETER_SET);
	printCircles(circles,DEER_PARAMETER_SET.imagePath);
	
	return 0;
}

void printCircles(std::vector<Circle*> circles,std::string imPath){
	 /// Read the image
	 Mat src = imread( imPath, 1 );

	if( !src.data )
	 { exit(1); }

	Mat blankMat = Mat::zeros(src.size(), src.type());


	for(int i=0 ; i<circles.size() ; i++ ){
		Circle* nextCircle = circles[i];
		
		cv::Point center(nextCircle->y,nextCircle->x);
		int R = circles[i]->R; int G = circles[i]->G; int B = circles[i]->B;

		circle( blankMat, center, circles[i]->radius, Scalar(R,G,B) );

	}
	

	
	imshow( "Hough Circle Transform Demo", blankMat );

	waitKey(0);
}




void extractNeigbours(std::vector<Circle*> &circleVect, float maxDistFromNeighbour){
	for(int i=0 ; i<circleVect.size() ; i++){
		for(int j=i+1 ; j<circleVect.size() ; j++){
			if( Circle::dist(*circleVect[i],*circleVect[j]) < maxDistFromNeighbour){

				bool goesThroughAnotherCircle = false;
				//if the line goes through another circle - they are noy neighbours!
				for(int k=0 ; k<circleVect.size() ; k++){
					if(k==i || k==j){
						continue;
					}
					
					float middleX = (circleVect[i]->x + circleVect[j]->x) / 2;
					float middleY = (circleVect[i]->y + circleVect[j]->y) / 2;

					Circle* tempCircle = circleVect[k];
					float distFromCircleCenter = sqrt((middleX-tempCircle->x)*(middleX-tempCircle->x) + (middleY-tempCircle->y)*(middleY-tempCircle->y));
					if(distFromCircleCenter - tempCircle->radius < 0){
						goesThroughAnotherCircle = true;
						break;
					}
				}
				if(goesThroughAnotherCircle){
					continue;
				}

				//else - update that the circles are neighbours
				circleVect[i]->neighbours.push_back(circleVect[j]->index);
				circleVect[j]->neighbours.push_back(circleVect[i]->index);
			}
		}
	}
}




bool doesCirclesIntersect(float x0, float y0, float r0, float x1, float y1, float r1){
    // This function checks for the intersection of two circles.
    // If one circle is wholly contained within the other a -1 is returned
    // If there is no intersection of the two circles a 0 is returned
    // If the circles intersect a 1 is returned and
    // the coordinates are placed in xi1, yi1, xi2, yi2
 
    // dx and dy are the vertical And horizontal distances between
    // the circle centers.
    float dx = x1 - x0;
    float dy = y1 - y0;
 
    // Determine the straight-Line distance between the centers.
    float d = sqrt((dy*dy) + (dx*dx));
 
 
    // Check for solvability.
    if (d > (r0 + r1)){
        //'no solution. circles do Not intersect
        return false;
	}
 
    if (d < abs(r0 - r1)){
    // no solution. one circle is contained in the other
        return true;
	}
 
    // 'point 2' is the point where the Line through the circle
    // intersection points crosses the Line between the circle
    // centers.
 
    // Determine the distance from point 0 To point 2.
    float a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d);
 
    // Determine the coordinates of point 2.
    float x2 = x0 + (dx * a/d);
    float y2 = y0 + (dy * a/d);
 
    // Determine the distance from point 2 To either of the
    // intersection points.
    float h = sqrt((r0*r0) - (a*a));
 
    // Now determine the offsets of the intersection points from
    // point 2.
    float rx = (0-dy) * (h/d);
    float ry = dx * (h/d);
 
    // Determine the absolute intersection points.
    float xi1 = x2 + rx;
    float xi2 = x2 - rx;
    float yi1 = y2 + ry;
    float yi2 = y2 - ry;
 
    return true;
}