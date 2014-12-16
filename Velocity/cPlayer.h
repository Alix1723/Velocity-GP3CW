#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "GameConstants.h"
#include "cModel.h"

//Player class to hold the position of the player's ship model.

class cPlayer : public cModel
{
public:
	cPlayer();
	~cPlayer();
	void update(float elapsedTime);
};

#endif