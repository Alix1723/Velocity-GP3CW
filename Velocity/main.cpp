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
#include "cObstacle.h"
#include "cSkyscraper.h"

#define FONT_SZ 42

using namespace std;

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
	cPlayer player;
	player.initialise(glm::vec3(0, 0, 0),0,glm::vec3(1,1,1),glm::vec3(0,0,0),0.0f, true);

	//Highway
	cModelLoader highwayModel;
	highwayModel.initialise("Models/Highway.obj");

	//Highway brackets
	cModelLoader highwayBracket;
	highwayBracket.initialise("Models/HighwayGate.obj");
	
	//Obstacles
	vector<cObstacle> obList;
	cModelLoader obstacleModel;
	obstacleModel.initialise("Models/Obstacle.obj");	

	float minSpawnDelay = 80.0f;  //Minimum delay between each spawn
	float maxSpawnDelay = 450.0f; //Maximum delay from last spawn (minimum + random value between 0 and this maximum);

	//Skyscrapers
	//vector<cSkysc> 

	//Font loading
	struct dtx_font *fntmain;
	fntmain = dtx_open_font("Fonts/eurostile.TTF", 0);
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);
	dtx_use_font(fntmain, FONT_SZ);

	//Camera
	bool isCameraTopDown = false;
	float cameraX = 0.0f;
	float cameraY = 40.0f;
	float cameraZ = 80.0f;

	//Movement variables
	float xOffset = 0.0f;
	float smoothxOffset = 0.0f; //Smoothing out X movement
	float maxDeadZone = 0.2f; //Threshold to prevent smoothing snapping back and forth
	float maxDelta = 5; //How quickly to move between lanes

	float distance = 0.0f; //Total distance travelled (used for gates and obstacles)

	//Distance before the first obstacles will spawn
	float nextObstacleSpawn = 1000;	

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events
		
		//The time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL();
		glClearColor(backgroundColours[0], backgroundColours[1], backgroundColours[2], 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Game is in action
		if (IsGamePlaying)
		{
			//Spawning new obstacles 
			if (distance > nextObstacleSpawn)
			{
				//Create a new obstacle object
				cObstacle* newObstacle = new cObstacle();
					newObstacle->setPosition(glm::vec3((int)(rand() % 3) * 30 - 30, 0, 200));
					newObstacle->setRotation(0);
					newObstacle->setScale(glm::vec3(1, 1, 1));
					newObstacle->setDirection(glm::vec3(0, 0, 0));
					newObstacle->setSpeed(0);
					newObstacle->setIsActive(true);
					newObstacle->setIsInPlay(true);    
					newObstacle->setStartZ(distance);

				obList.push_back(*newObstacle);
				
				//Determine when the next obstacle will spawn
				nextObstacleSpawn = distance + minSpawnDelay + (fmod(rand(),maxSpawnDelay));
			}

			//Updating horizontal (x) position
			xOffset = highwayLane*30.0f - 60.0f;

			//Smoothing x movement
			if (xOffset != smoothxOffset){
				float difference = (xOffset - smoothxOffset);

				//Avoid Zeno's "Arrow Paradox" with this check
				if ((difference < maxDeadZone) & (difference > -maxDeadZone))
				{
					smoothxOffset = xOffset;
				}
				else
				{	//Divide the difference by 2
					smoothxOffset += difference / 2;
				}
			}

			//Increasing distance with time
			distance = distance + elapsedTime * 700;
		}

		//Camera angle
		if (cameraToggle)
		{   
			gluLookAt(0, 400, -109, 0, 0, -110, 0, 1, 0); //Top-down
		}
		else
		{
			gluLookAt(cameraX + smoothxOffset/2, cameraY, cameraZ, smoothxOffset, 20, 0, 0, 1, 0); //Third person 'follow' camera
		}

		//Highway model
		highwayModel.renderMdl(glm::vec3(0, 0, 0));

		//Looping highway bracket
		highwayBracket.renderMdl(glm::vec3(0, 0, 1200 - fmod(distance,1400)));

		//Obstacles
		for(int j = 0; j < obList.size(); j++)
		{
			//Checking if the player has moved past the obstacle, and scored a point
			if ((obList[j].getPosition().z < 0) & (obList[j].getIsInPlay() == true))
			{
				obList[j].setIsInPlay(false);
				playerPoints++;
			}
			//Collision with player
			if (obList[j].SphereSphereCollision(player.getPosition(), 10) & (obList[j].getIsInPlay() == true))
			{
				IsGamePlaying = false;
				distance = 0;
				xOffset = 0;
				playerHighScore = playerPoints;
				playerPoints = 0;
				obList.clear();
				break;
			}

			//Check if the obstacle has moved off the world
			if (obList[j].getPosition().z < -200)
			{
				obList.erase((obList.begin() + j));
				break;
			}
			//Otherwise, move the obstacle down the highway
			else
			{
				obList[j].setPosition(glm::vec3(obList[j].getPosition().x, 0, 1200 - (distance - obList[j].getStartZ())));
				obstacleModel.renderMdl(obList[j].getPosition());
				obList[j].update(elapsedTime);
			}
		}
		
		//Player positioning
		player.setPosition(glm::vec3(smoothxOffset, IsGamePlaying ? 0 : -1000, 0));
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
		
		//Points display
		glTranslatef(50, 50, 0);
		std::string out = "POINTS : " + std::to_string(playerPoints);
		dtx_string(out.c_str());

		//Camera indicator
		glTranslatef(950, 0, 0);
		dtx_string(cameraToggle ? "[C] FOLLOW" : "[C] TOP");

		if (!IsGamePlaying)
		{
			//Start/finish display
			glTranslatef(-500, 200, 0);
			std::string hiscore = "HIGH SCORE : " + std::to_string(playerHighScore) + "\nPRESS SPACE TO PLAY";
			dtx_string(hiscore.c_str());
		}

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
