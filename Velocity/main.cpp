#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include "GameConstants.h"
#include <windows.h>
#include <vector>
#include <ctime>
#include <string>
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cModel.h"
#include "libdrawtext-0.2.1\drawtext.h"
#include "cMouseControl.h"
#include "cModelLoader.h"
#include "cPlayer.h"

#define FONT_SZ 24

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int windowBPP = 16;

    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

    //The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);


    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL()) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Random seed
	srand(time(NULL));
	
	//Player model
	cModelLoader cobraModel;
	cobraModel.initialise("Models/MkIIICobra.obj");

	cPlayer player;// = new cPlayer();
	player.initialise(glm::vec3(0, 0, 0),0,glm::vec3(1,1,1),glm::vec3(0,0,0),0.0f, true);

	//.cModelLoader tardisModel;
	//tardisModel.initialise("Models/tardis.obj");

	//Font loading
	struct dtx_font *fntmain;
	fntmain = dtx_open_font("Fonts/eurostile.TTF", 0);
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);
	dtx_use_font(fntmain, FONT_SZ);

	float cameraX = 0.0f;
	float cameraY = 0.0f;
	float cameraZ = 50.0f;

	int playerPoints = 0;
	
	//Mouse input
	cMouseControl* mouseInput = new cMouseControl();

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events
		
		//The time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL();
		glClearColor(0.1f,0.1f,0.1f,0.1f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 15, 50, 0, 0, 0, 0, 1, 0);


		//Updating the game world

		//Inputs
		if (pgmWNDMgr->isWindowFocused()) //Check if window has focus first
		{
			//Some DIY Input handling
			std::vector<float> mouseDelta = mouseInput->GetMouseDelta();

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (mouseInput->IsTracking())
				{	
					glm::vec3 current = player.getPosition();
					glm::vec3 tr = glm::vec3(mouseDelta[0], -mouseDelta[1], 0);
					player.setPosition(current + tr);
				}
				else
				{
					mouseInput->StartTrackingMouse();
					ShowCursor(false);
				}
			}
			else
			{
				if (!GetAsyncKeyState(VK_LBUTTON) & mouseInput->IsTracking())
				{
					mouseInput->StopTrackingMouse();
					ShowCursor(true);
				}
			}
		}

		//Player model
		cobraModel.renderMdl(player.getPosition());
		player.update(elapsedTime);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, windowWidth, 0, windowHeight);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glPushAttrib(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);
		
		//Text
		glTranslatef(50, 50, 0);
		glColor3f(1, 1, 1);
		std::string out = "POINTS : " + std::to_string(playerPoints);
		dtx_string(out.c_str());

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		//Swap the image buffer
		pgmWNDMgr->swapBuffers();
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
