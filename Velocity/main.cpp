#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY 

/*
	"VELOCITY" 
	Alasdair Gibson S1111320
	Games Programming 3 Coursework
*/

#include "GameConstants.h"
#include <windows.h>
#include <vector>
#include <ctime>
#include <string>
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cModel.h"
#include "libdrawtext-0.2.1\drawtext.h"
#include "cModelLoader.h"
#include "cPlayer.h"
#include "cObstacle.h"
#include "cSkyscraper.h"
#include "cSound.h"
#include "cXboxController.h"

#define FONT_SZ 42
#define FONT_SZ_SMALL 32

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{
    //Window settings
    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int windowBPP = 16;

    //Window instance
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

    //OpenGL window
    windowOGL theOGLWnd;

    //Attach OpenGL to current window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails
        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL()) //Initialize window
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Randomized seed
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
	vector<cSkyscraper> skyList;
	cModelLoader skyscraperModel;
	skyscraperModel.initialise("Models/Skyscraper.obj");
	int numSkyscrapers = 10;
	float width = 600; //Horizontal range of skyscraper placement
	float minWidth = 140; //Minumim distance from centre (highway)
	float length = 4000; //Forward (Z) distance of skyscraper placement
	for (int i = 0; i < numSkyscrapers; i++)
	{
		float rndX = fmod(rand(),width*2) - width;
		if (rndX > -minWidth & rndX < 0){ rndX = minWidth; } //Clamping X above/below the minimum distance
		if (rndX < minWidth & rndX > 0){ rndX = minWidth; }
		float rndZ = fmod(rand(), length);
		
		cSkyscraper sk;
		sk.initialise(glm::vec3(rndX, 0, 1200 + rndZ), 0, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 0.0f, true);
		sk.setRotation(rndZ * 100.0f);
		sk.setStartZ(1200 + rndZ);

		skyList.push_back(sk);
	}

	//Audio loading
	//FORMAT: WAV, PCM, 44.1khz, 16 bit
	cSound pointsSound;
	pointsSound.createContext(); //NOTE: Only do this once, else sounds will fail to play/become distorted and broken.
	pointsSound.loadWAVFile("Audio/scorePoint.wav");

	cSound explodeSound;
	explodeSound.loadWAVFile("Audio/shipExplode.wav");

	cSound startSound;
	startSound.loadWAVFile("Audio/powerUp.wav");

	//"Piercing the Sky" by Magnus 'Souleye' Pålsson. http://souleyedigitalmusic.bandcamp.com/track/piercing-the-sky
	cSound backgroundMusic;
	backgroundMusic.loadWAVFile("Audio/PiercingTheSky.wav");
	backgroundMusic.playAudio(AL_TRUE); //Set to loop
	bool bgmPlaying = true;


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

	float distance = 0.0f; //Total distance travelled (used for gates, obstacles and skyscrapers)
	float startSpeed = 700.0f; //Speed to start at


	//Distance before the first obstacles will spawn
	float nextObstacleSpawn = 1000;	


	//Controller input
	cXboxController* controller1 = new cXboxController(0);


    //Main game loop, runs until the window shuts down.
	while (pgmWNDMgr->isWNDRunning())
    {
		//Process window events (including keyboard inputs)
		pgmWNDMgr->processWNDEvents(); 

		//Calculate the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();
		
		//Process controller inputs
		controller1->Update(elapsedTime); 
		if (!controllerEnabled)
		{	//Enabling controller inputs when A is pressed
			if (controller1->GetButtonDown(1))
			{
				controllerEnabled = 1;
			}
		}
		else
		{
			//Start
			if (controller1->GetButtonDown(1) & !isGamePlaying)
			{
				isGamePlaying = true;
				controller1->Vibrate(0,0);
			}

			//Left
			if (controller1->GetButtonDown(5) & highwayLane>1)
			{
				highwayLane--;
			}
			//Right
			if (controller1->GetButtonDown(6) & highwayLane<3)
			{
				highwayLane++;
			}

			//Camera
			if (controller1->GetButtonDown(3))
			{
				cameraToggle = !cameraToggle;
			}

			//Audio mute/unmute
			if (controller1->GetButtonDown(2))
			{
				isSoundEnabled = !isSoundEnabled;
			}
		}
		
		//Clearing the window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL();
		glClearColor(backgroundColours[0], backgroundColours[1], backgroundColours[2], 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		//Game has started
		if (isGamePlaying)
		{
			//Play start sound once
			if (distance <= 0 & isSoundEnabled)
			{
				startSound.playAudio(AL_FALSE);
			}

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
				{	//Divide the difference by 3
					smoothxOffset += difference / 3;
				}
			}

			//Increasing distance with time, multiplied by score
			distance = distance + elapsedTime * (startSpeed + playerPoints * 4);
		}

		//Camera angle
		if (cameraToggle)
		{   
			gluLookAt(0, 500, -150, 0, 0, -155, 0, 1, 0); //Top-down
		}
		else
		{
			gluLookAt(cameraX + smoothxOffset/2, cameraY, cameraZ, smoothxOffset, 20, 0, 0, 1, 0); //Third person 'follow' camera
		}

		//Highway model
		highwayModel.renderMdl(glm::vec3(0, 0, 0),0.0f);

		//Looping highway bracket
		highwayBracket.renderMdl(glm::vec3(0, 0, 1200 - fmod(distance,1400)),0.0f);

		//Obstacles
		for(int j = 0; j < obList.size(); j++)
		{
			//Checking if the player has moved past the obstacle, and scored a point
			if ((obList[j].getPosition().z < 0) & (obList[j].getIsInPlay() == true))
			{
				obList[j].setIsInPlay(false);
				if (isSoundEnabled){ pointsSound.playAudio(AL_FALSE); }
				pointsSound.setSoundPitch(0.5f + (playerPoints / 100.0f));
				playerPoints++;
			}
			//Collision with player
			if (obList[j].SphereSphereCollision(player.getPosition(), 12) )
			{	
				isGamePlaying = false;
				if (isSoundEnabled){ explodeSound.playAudio(AL_FALSE); }
				
				//Force feedback
				if (controllerEnabled)
				{
					controller1->Vibrate(65535, 65535);
				}

				//Set highscore
				if (playerHighScore < playerPoints)
				{
					playerHighScore = playerPoints;
				}

				//Reset everything to initial values/state
				playerPoints = distance = xOffset = smoothxOffset = 0;
				obList.clear();
				nextObstacleSpawn = 1000;
				for (int i = 0; i < numSkyscrapers; i++)
				{
					//Place all skyscrapers back at start
					float rndZ = fmod(rand(), length);
					skyList[i].setStartZ(1200 + rndZ);
				}
				break;
			}

			//Check if the obstacle has moved off the world
			if (obList[j].getPosition().z < -200)
			{
				obList.erase((obList.begin() + j));
				//break; Note: this causes all objects to flicker, whereas this only causes the next object in the array to flicker. (Use foreach/non-indexed loop?)
			}
			//Otherwise, move the obstacle down the highway
			else
			{
				obList[j].setPosition(glm::vec3(obList[j].getPosition().x, 0, 1200 - (distance - obList[j].getStartZ())));
				obstacleModel.renderMdl(obList[j].getPosition(),0.0f);
				obList[j].update(elapsedTime);
			}
		}

		//Skyscrapers
		for (int k = 0; k < skyList.size(); k++)
		{
			//Reset the starting Z value to loop the skyscraper back
			if (skyList[k].getPosition().z < -200)
			{
				skyList[k].setStartZ(distance + (fmod(rand(), 5000)));
			}

			skyList[k].setPosition(glm::vec3(skyList[k].getPosition().x, -100, 1400 - (distance - skyList[k].getStartZ())));
			skyscraperModel.renderMdl(skyList[k].getPosition(),skyList[k].getRotation());
			skyList[k].update(elapsedTime);
		}
		
		//Player model rendering
		player.setPosition(glm::vec3(smoothxOffset, isGamePlaying ? 0 : -1000, 0));
		cobraModel.renderMdl(player.getPosition(),0.0f);
		player.update(elapsedTime);

		//Toggling background music when isSoundEnabled changes
		if (!isSoundEnabled & bgmPlaying)
		{
			backgroundMusic.stopAudio();
			pointsSound.stopAudio(); //Also stop any other sound effects
			startSound.stopAudio(); 
			explodeSound.stopAudio();

			bgmPlaying = false;
		}
		else if (isSoundEnabled & !bgmPlaying)
		{
			backgroundMusic.playAudio(AL_TRUE);
			bgmPlaying = true;
		}

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
		glTranslatef(50, 80, 0);
		std::string out = "POINTS : " + std::to_string(playerPoints);
		dtx_string(out.c_str());
		glTranslatef(-50, -50, 0);

		//Camera & Audio indicators
		glTranslatef(1100, 50, 0);
		dtx_prepare_range(fntmain, FONT_SZ_SMALL, 0, 256);
		dtx_use_font(fntmain, FONT_SZ_SMALL);
		if (controllerEnabled)
		{
			dtx_string(cameraToggle ? "[X] FOLLOW" : "[X] TOP");
			dtx_string(isSoundEnabled ? "\n[B] MUTE" : "\n[B] UNMUTE");
		}
		else
		{
			dtx_string(cameraToggle ? "[C] FOLLOW" : "[C] TOP");
			dtx_string(isSoundEnabled ? "\n[V] MUTE" : "\n[V] UNMUTE");
		}
		glTranslatef(-1100, -50, 0);
		
		//Start/finish display
		dtx_prepare_range(fntmain, FONT_SZ, 0, 256);
		dtx_use_font(fntmain, FONT_SZ);
		glTranslatef(150, 350, 0);
		if (!isGamePlaying)
		{
			std::string hiscore = "HIGH SCORE : " + std::to_string(playerHighScore);
			dtx_string(hiscore.c_str());

			dtx_prepare_range(fntmain, FONT_SZ_SMALL, 0, 256);
			dtx_use_font(fntmain, FONT_SZ_SMALL);
			dtx_string(controllerEnabled ? "\n\nUSE [LEFT STICK] / [DPAD LEFT] AND [DPAD RIGHT] TO AVOID OBSTACLES" : "\n\nUSE [A] AND [D] / [LEFT] AND [RIGHT] TO AVOID OBSTACLES");
			
			dtx_prepare_range(fntmain, FONT_SZ, 0, 256);
			dtx_use_font(fntmain, FONT_SZ);
			dtx_string(controllerEnabled ? "\n\n\nPRESS [A] TO PLAY" : "\n\n\nPRESS [SPACE] TO PLAY"); //Modify display text to suit input method

			//Title
			glTranslatef(380, 200, 0);
			dtx_string("[ VELOCITY ]");
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
	delete(controller1); //Remove controller object

    return 0; //Return success
}
