#include "PolyFill.h"
class Figure{
public:
	Figure();
	bool selected(point_type point);
	virtual void printEdges();
};