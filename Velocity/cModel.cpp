#include "cModel.h"

cModel::cModel()
{
	m_mdlPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_mdlRotation = 0.0f;
	m_mdlVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_model = NULL;
}

void cModel::setPosition(glm::vec3 mdlPosition)
{
	m_mdlPosition = mdlPosition;
}

void cModel::setRotation(GLfloat mdlRotation)
{
	m_mdlRotation = mdlRotation;
}

void cModel::setVelocity(glm::vec3 mdlVelocity)
{
	m_mdlVelocity = mdlVelocity;
}

glm::vec3 cModel::getPosition()
{
	return m_mdlPosition;
}

GLfloat cModel::getRotation()
{
	return m_mdlRotation;
}

glm::vec3 cModel::getVelocity()
{
	return m_mdlVelocity;
}

void cModel::initialise(const char* mdlFilename, glm::vec3 mdlPosition, GLfloat mdlRotation, glm::vec3 mdlVelocity)
{
	m_model = glmReadOBJ(mdlFilename);
	glmVertexNormals(m_model, 180.0, false);
	setPosition(mdlPosition);
	setRotation(mdlRotation);
	setVelocity(mdlVelocity);
	glm::vec3 mdlPos = getPosition();
	GLfloat mdlRot = getRotation();
	glRotatef(mdlRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(mdlPos.x, mdlPos.y, mdlPos.z);

}
void cModel::update(float elapsedTime)
{
	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = -(float)glm::sin(m_mdlRotation);
	mdlVelocityAdd.z = -(float)glm::cos(m_mdlRotation);
	mdlVelocityAdd *= elapsedTime;

	//m_mdlVelocity *=  mdlVelocityAdd;// elapsedTime;

	m_mdlPosition += m_mdlVelocity;

	glm::vec3 mdlPos = getVelocity();
	glTranslatef(mdlPos.x, mdlPos.y, mdlPos.z);

}

void cModel::renderMdl()
{

	glPushMatrix();
	//transformations here...
	glmDraw(m_model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	// Render Text
	glPopMatrix();
}

cModel::~cModel()
{

}