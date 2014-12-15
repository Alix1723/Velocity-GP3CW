#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

#define GLM_FORCE_RADIANS

#include <stdlib.h>
#include <Windows.h>
#include <GL\glut.h>
/*#include <al.h>
#include <alc.h>
#include <alut.h>*/
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\geometric.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\quaternion.hpp>
#include "glm-0.3.2\glm\glm.h"

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "Velocity"

struct mdlDimensions{
	float s_mdlWidth, s_mdlheight, s_mdldepth;
};

const float PLAYFIELDX = 100.0f;
const float PLAYFIELDZ = 300.0f;

extern bool IsGamePlaying;
extern int drawMode;
extern int highwayLane;
extern bool cameraToggle;
extern float backgroundColours[3];
extern int playerPoints;
extern int playerHighScore;



#endif