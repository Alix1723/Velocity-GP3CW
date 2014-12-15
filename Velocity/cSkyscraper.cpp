#include "cSkyscraper.h"

cSkyscraper::cSkyscraper() : cModel()
{
	//Constructor
}

void cSkyscraper::update(float elapsedTime)
{
	//Update here

}

void cSkyscraper::setStartZ(float startz)
{
	f_startZ = startz;
}

float cSkyscraper::getStartZ()
{
	return f_startZ;
}

cSkyscraper::~cSkyscraper()
{
	//Destructor
}