#include "BoardImprover.h"
#include "Board.h"
#include "CircleExtractor.h"

bool isNewCircleValidHere(Board& b, int x, int y, int r){
	std::vector<Circle*>::iterator it;
	for (it = b.getCirclesRef().begin(); it != b.getCirclesRef().end(); it++) {
		if(doesCirclesIntersect(x,y,r,(*it)->x,(*it)->y,(*it)->radius)){
			return false;
		}
	}
	return true;
}

bool addCircle(Board& b, int x, int y, int r){
	if(! isNewCircleValidHere(b,x,y,r) ){
		return false;
	}


	b.getCirclesRef().push_back(circle);
}



void removeCircle(Board& b, Circle* circle){
	std::vector<Circle*>::iterator it;
    for (it = b.getCirclesRef().begin(); it != b.getCirclesRef().end(); /* DONT increment here*/) {
		if(*it == circle){
			it = b.getCirclesRef().erase(it);
		}else
        {
            it++;
        }
    }

	b.getCirclesRef().erase(it);

}