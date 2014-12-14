#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "GameConstants.h"
#include "cModel.h"

class cPlayer : public cModel
{

public:
	cPlayer();
	~cPlayer();

	void update(float elapsedTime);

private:
};

#endif