#include "cObstacle.h"

cObstacle::cObstacle() : cModel()
{
	//Constructor
}

void cObstacle::update(float elapsedTime)// : cModel()
{
	//Update here
}

void cObstacle::setIsInPlay(bool in)
{
	b_isInPlay = in;
}

bool cObstacle::getIsInPlay()
{
	return b_isInPlay;
}

void cObstacle::setStartZ(float startz)
{
	f_startZ = startz;
}

float cObstacle::getStartZ()
{
	return f_startZ;
}
cObstacle::~cObstacle()
{
	//Destructor
}
