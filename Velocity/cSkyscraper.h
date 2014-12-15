#ifndef _CSKYSCRAPER_H
#define _CSKYSCRAPER_H

#include "GameConstants.h"
#include "cModel.h"

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