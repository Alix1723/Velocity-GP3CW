#ifndef _COBSTACLE_H
#define _COBSTACLE_H

#include "GameConstants.h"
#include "cModel.h"

//Class to hold the position of each obstacle, as well as their initial spawn position and whether they are still 'valid'

class cObstacle : public cModel
{
public:
	cObstacle();
	~cObstacle();

	void update(float elapsedTime); //Inherited from cModel

	void setIsInPlay(bool in); //Check whether the obstacle is still valid (For point scoring purposes)
	bool getIsInPlay();

	void setStartZ(float startz); //Set to the distance the player is at when the obstacle is spawned, in order to move the obstacle on the Z axis
	float getStartZ();

private:
	bool b_isInPlay;
	float f_startZ;
};

#endif