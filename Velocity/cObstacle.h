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
};

#endif