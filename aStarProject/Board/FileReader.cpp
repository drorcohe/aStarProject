#include "FileReader.h"


void writeBoardToFile(std::vector<Circle*> circles, std::string fileName){
	
	std::ofstream outputFile;
	outputFile.open(fileName);


	for(int i=0 ; i<circles.size() ; i++ ){
		Circle* circle = circles[i];
		outputFile<< circle->y <<" " << circle ->x <<" " << circle->radius << " " <<circle->R <<" " <<circle->G<<" " << circle->B << " " << circle->index <<std::endl;
	}

	outputFile<<NEIGBOURS_HEADLINE<<std::endl;
	for(int i=0 ; i<circles.size() ; i++ ){
		Circle* circle = circles[i];
		outputFile<<i<< " : ";
		for(int j=0 ; j < circle->neighbours.size() ; j++){
			outputFile<<" " << circle->neighbours[j];
		}
		outputFile<<std::endl;
	}
	outputFile.close();

}


void readBoardFromFile(std::string fileName,std::vector<Circle*>& circles){
	
	

	std::string line;
	std::ifstream inputFile (fileName);
	int maxCircleIndex = 0;
	if (inputFile.is_open())
	 {
		
		 
		//extracts circles, until reaches to NEIGBOURS_HEADLINE
		while ( getline (inputFile,line)) 
		{
			if(line.compare(NEIGBOURS_HEADLINE) == 0){
				break;
			}
			std::istringstream ss(line);
			int x ,y, radius, R,G,B, index;

			ss >> y;
			ss >> x;
			ss >> radius;
			ss >> R;
			ss >> G;
			ss >> B;
			ss >> index;

			Circle* nextCircle = new Circle(x,y,index,radius,R,G,B);
			circles.push_back(nextCircle);
			maxCircleIndex = std::max(maxCircleIndex,index);


		}



		//extracts neighbours
		while ( getline (inputFile,line)){
			std::istringstream ss(line);
			char temp;
		
			int currentIndex,neighbourIndex;
			ss >> currentIndex;
			Circle* currentCircle = circles[currentIndex];
			ss >> temp; //ignores the ":"

			int prevNeighbourIndex = -1;
			while(ss.str().find_first_not_of(' ') != std::string::npos){
				ss >> neighbourIndex;
				if(prevNeighbourIndex==neighbourIndex){
					break;
				}
				currentCircle->neighbours.push_back(neighbourIndex);
				prevNeighbourIndex = neighbourIndex;
			}

		}

		inputFile.close();
	}



}