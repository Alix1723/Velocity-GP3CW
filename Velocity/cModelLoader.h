#ifndef _CMODELLOADER_H
#define _CMODELLOADER_H

#include "GameConstants.h"

class cModelLoader
{
public:
	cModelLoader();

	void initialise(const char* mdlFilename);
	void renderMdl(glm::vec3 mdlPosition);

	~cModelLoader();
private:
	// Set the position of the model in world space, and set the rotation.
	GLMmodel* m_model;

};
#endif