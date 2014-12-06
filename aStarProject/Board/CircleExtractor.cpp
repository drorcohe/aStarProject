#include "CircleExtractor.h"
#include "Board.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include "FileReader.h"

using namespace cv;

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





std::vector<Circle*> getCirclesFromImage(std::string path){
	Mat src, src_orig, src_gray, out;

  /// Read the image
  src = imread( path, 1 );

  if( !src.data )
    { exit(1); }

  /// Convert it to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  cv::Mat thresh;
  //cv::threshold(src_gray,src_gray,100,255,src_gray.type()); monro

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;
  std::vector<Circle*> outCircles;
  //1, 2 ,2,100,1
  //HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 2 ,2,25,1,30); // min and max radius
 // HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 2 ,2,11,2,11); monro
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, 6 ,2,13,2,17); 
  /// Apply the Hough Transform to find the circles
  //HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );

  /// Draw the circles detected
	int circleCounter = 0;
  for( size_t i = 0; i < circles.size(); i++ )
  {
	  
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
	 // Vec3i value = image.at<cv::Vec3b>(y,x)[0];
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

	extractNeigbours(outCircles);
	return outCircles;


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




void extractNeigbours(std::vector<Circle*> &circleVect){
	for(int i=0 ; i<circleVect.size() ; i++){
		for(int j=i+1 ; j<circleVect.size() ; j++){
			if( Circle::dist(*circleVect[i],*circleVect[j]) < MAX_DIST_FROM_NEIGHBOUR){
				circleVect[i]->neighbours.push_back(circleVect[j]->index);
				circleVect[j]->neighbours.push_back(circleVect[i]->index);
			}
		}
	}
}

