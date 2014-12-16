#ifndef _CMODELLOADER_H
#define _CMODELLOADER_H

#include "GameConstants.h"

//Loads models from .obj files. Calling renderMdl() will render one instance of a particular model at that position, and so can be used multiple times for multiple identical models.
//Code duplicated from Lab9 by Bobby Law.

class cModelLoader
{
public:
	cModelLoader();

	void initialise(const char* mdlFilename);
	void renderMdl(glm::vec3 mdlPosition, float rotationAngle);

	~cModelLoader();
private:
	// Set the position of the model in world space, and set the rotation.
	GLMmodel* m_model;

};
#endif