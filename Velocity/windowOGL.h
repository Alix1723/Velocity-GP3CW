#ifndef _WINDOWOGL_H
#define _WINDOWOGL_H

#include <windows.h>
#include <GL/glut.h>

//Handles the OpenGL context/window creation and rendering.
//Code (mostly) duplicated from Labs by Bobby Law.

class windowOGL 
{
public:
    windowOGL();

    bool initOGL();
	void renderOGL(float rotAngle);
    void shutdown();
    void onResize(int width, int height);
};

#endif
