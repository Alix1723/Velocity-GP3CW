#include "cModelLoader.h"

cModelLoader::cModelLoader()
{
	m_model = NULL;
}


void cModelLoader::initialise(const char* mdlFilename)
{
	m_model = glmReadOBJ(mdlFilename);
	glmVertexNormals(m_model, 180.0, false);
}

void cModelLoader::renderMdl(glm::vec3 mdlPosition)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
	//transformations here...
	glTranslatef(mdlPosition.x, mdlPosition.y, -mdlPosition.z);

	glRotatef(rotationAngle, 0, 1, 0);
	glmDraw(m_model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	// Render Text
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

cModelLoader::~cModelLoader()
{

}
