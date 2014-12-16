#ifndef _CSKYSCRAPER_H
#define _CSKYSCRAPER_H

#include "GameConstants.h"
#include "cModel.h"

//Class to hold the background Skyscrapers, similar to cObstacle, but with no validity/points check.

class cSkyscraper : public cModel
{

public:
	cSkyscraper();
	~cSkyscraper();
	
	void update(float elapsedTime);

	void setStartZ(float startz);
	float getStartZ();

private:
	float f_startZ;
};

#endif