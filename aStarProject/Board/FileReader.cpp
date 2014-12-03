#include "FileReader.h"


void writeCirclesToFile(std::vector<Circle*> circles, std::string fileName){
	std::ofstream outputFile;
	outputFile.open(fileName);
	for(int i=0 ; i<circles.size() ; i++ ){
		Circle* circle = circles[i];
		outputFile<< circle->x <<" " << circle ->y <<" " << circle->radius << " " <<circle->R <<" " <<circle->G<<" " << circle->B << std::endl;
	}

	outputFile.close();

}


std::vector<Circle*> readCirclesFromFile(std::string fileName){
	std::vector<Circle*> retVec;
	

	std::string line;
	std::ifstream inputFile (fileName);
	if (inputFile.is_open())
	 {
		int counter = 0;
		while ( getline (inputFile,line) )
		{

			std::istringstream ss(line);
			int x ,y, radius, R,G,B;

			ss >> x;

			ss >> y;
			ss >> radius;
			ss >> R;
			ss >> G;
			ss >> B;
			
			Circle* nextCircle = new Circle(x,y,counter,radius,R,G,B);
			retVec.push_back(nextCircle);
			counter++;

		}
		inputFile.close();
	}

 
	return retVec;


}