#include "windowOGL.h"
#include "GameConstants.h"

windowOGL::windowOGL()
{

}

bool windowOGL::initOGL()
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClearStencil(0);                          // clear stencil buffer
	glClearDepth(1.0f);                         // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = { .5, 1., 1., 0. };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	onResize(1280, 720);

	//Fog (*http://content.gpwiki.org/index.php/OpenGL:Tutorials:Tutorial_Framework:Light_and_Fog)
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, backgroundColours);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 500.0f);
	glFogf(GL_FOG_END, 1000.0f);

    //Return success
    return true;
}

void windowOGL::renderOGL(float rotAngle)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void windowOGL::shutdown()
{

}

void windowOGL::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 16.0f / 9.0f, 1.0f, 5000.0f);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}
