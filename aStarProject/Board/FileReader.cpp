#include "FileReader.h"


void writeBoardToFile(std::vector<Circle*> circles, std::string fileName, int startCircle, int endCircle){
	
	std::ofstream outputFile;
	outputFile.open(fileName);

	if(startCircle!=-1 && endCircle != -1){
		outputFile << BOARD_START_END_HEADLINE << std::endl;
		outputFile<<startCircle << " " <<endCircle << std::endl;
	}


	for(int i=0 ; i<circles.size() ; i++ ){
		Circle* circle = circles[i];
		outputFile<< circle->x <<" " << circle ->y <<" " << circle->radius << " " <<circle->R <<" " <<circle->G<<" " << circle->B << " " << circle->index <<std::endl;
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


std::vector<Circle*> readBoardFromFile(std::string fileName, int& startCircle, int& endCircle){
	std::vector<Circle*> retVec;
	

	std::string line;
	std::ifstream inputFile (fileName);
	int maxCircleIndex = 0;
	if (inputFile.is_open())
	 {
		
		 if(getline (inputFile,line)){
			 if(line.compare(std::string(BOARD_START_END_HEADLINE)) == 0){
				getline (inputFile,line);
				std::istringstream ss(line);
				ss >> startCircle;
				ss >> endCircle;
				getline (inputFile,line);
			}
		 }
		//extracts circles, until reaches to NEIGBOURS_HEADLINE
		do 
		{
			if(line.compare(NEIGBOURS_HEADLINE) == 0){
				break;
			}
			std::istringstream ss(line);
			int x ,y, radius, R,G,B, index;

			ss >> x;
			ss >> y;
			ss >> radius;
			ss >> R;
			ss >> G;
			ss >> B;
			ss >> index;

			Circle* nextCircle = new Circle(x,y,index,radius,R,G,B);
			retVec.push_back(nextCircle);
			maxCircleIndex = std::max(maxCircleIndex,index);


		}while ( getline (inputFile,line));



		//extracts neighbours
		while ( getline (inputFile,line)){
			std::istringstream ss(line);
			char temp;
		
			int currentIndex,neighbourIndex;
			ss >> currentIndex;
			Circle* currentCircle = retVec[currentIndex];
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
	return retVec;


}