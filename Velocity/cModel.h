#ifndef _CMODEL_H
#define _CMODEL_H

#include "GameConstants.h"

class cModel
{
public:
	cModel();
	void setPosition(glm::vec3 mdlPosition);
	void setRotation(GLfloat mdlRotation);
	void setVelocity(glm::vec3 mdlVelocity);

	glm::vec3 getPosition();
	GLfloat getRotation();
	glm::vec3 getVelocity();

	void initialise(const char* mdlFilename, glm::vec3 mdlPosition, GLfloat mdlRotation, glm::vec3 mdlVelocity);
	void update(float elapsedTime);
	void renderMdl();

	~cModel();
private:
	// Set the position of the model in world space, and set the rotation.
	glm::vec3 m_mdlPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat m_mdlRotation = 0.0f;
	glm::vec3 m_mdlVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	GLMmodel* m_model;

};
#endif