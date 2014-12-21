#include "FileReader.h"


void writeBoardToFile(std::vector<Circle*> circles, int w, int h, std::vector<PATH_DETAIL> paths, std::string fileName){
	
	std::ofstream outputFile;
	outputFile.open(fileName);

	outputFile<<" " << w <<" " << h  << " " << std::endl;

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


	outputFile<< RECOMMENDED_PATHS_HEADLINE <<std::endl;
	for(int i=0 ; i<paths.size() ; i++){
		PATH_DETAIL nextPath = paths[i];
		outputFile << "s " << nextPath.start  <<std::endl  << "e ";

		for(int j = 0 ; j<nextPath.endPoints.size() ; j++){
			outputFile << " " << nextPath.endPoints[j];
		}
		outputFile<<std::endl << "d";
		for(int j = 0 ; j<nextPath.directions.size() ; j++){
			std::string nextStr = nextPath.directions[j]==RIGHT? "r":"l";
			outputFile << " " << nextStr;
		}
		outputFile<< std::endl;
	}

	outputFile.close();

}


void readBoardFromFile(std::string fileName,std::vector<Circle*>& circles,int& w, int& h, std::vector<PATH_DETAIL>& paths){
	
	

	std::string line;
	std::ifstream inputFile (fileName);
	int maxCircleIndex = 0;
	if (inputFile.is_open())
	 {

		getline (inputFile,line);
		std::istringstream ss(line);
		std::string wStr, hStr;

		ss >>wStr;
		ss >> hStr; 
		w = std::stoi( wStr );
		h = std::stoi( hStr );
		//ss >> w; 

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
			if(line.compare(RECOMMENDED_PATHS_HEADLINE) == 0){
				break;
			}
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


		paths = std::vector<PATH_DETAIL>();

		//extracts recommended paths
		//S %d  \n E %d %d \n d %c %c ...
		while(getline (inputFile,line)){
			std::istringstream ss(line);
			int start;
			std::vector<int> endPoints;
			std::vector<Direction> directions;
			int temp; 
			std::string tempC;

			ss >> tempC; //S
			ss >> start;
			
			getline (inputFile,line);
			ss = std::istringstream(line);
			ss >> tempC; //e
			
			while(ss.str().find_first_not_of(' ') != std::string::npos && ss.str().find_first_not_of('.') != std::string::npos){
				if(!(ss >> temp))
					break;
				endPoints.push_back(temp);
			}
			
			getline (inputFile,line);
			ss = std::istringstream(line);
			ss >> tempC; //d
			while(ss.str().find_first_not_of(' ') != std::string::npos && ss.str().find_first_not_of('.') != std::string::npos){
				if(!(ss >> tempC))
					break;
				Direction nextDirection = tempC[0]=='r'? RIGHT : LEFT;
				directions.push_back(nextDirection);
			}

			PATH_DETAIL pd; 
			pd.directions = directions;
			pd.endPoints = endPoints;
			pd.start = start;
			paths.push_back(pd);
		}
		

		inputFile.close();
	}



}