#ifndef _COBSTACLE_H
#define _COBSTACLE_H

#include "GameConstants.h"
#include "cModel.h"

class cObstacle : public cModel
{
public:
	cObstacle();
	~cObstacle();

	void update(float elapsedTime);

	void setIsInPlay(bool in);
	bool getIsInPlay();

	void setStartZ(float startz);
	float getStartZ();

private:
	bool b_isInPlay;
	float f_startZ;
};

#endif