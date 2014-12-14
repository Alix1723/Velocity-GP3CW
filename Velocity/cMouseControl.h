#ifndef _CMOUSECONTROL_H
#define _CMOUSECONTROL_H

#include "GameConstants.h"
#include <vector>

class cMouseControl
{
public:
	cMouseControl();
	~cMouseControl();

	void StartTrackingMouse();
	void StopTrackingMouse();
	bool IsTracking();

	//Call this to get the last mouse movement
	std::vector<float> GetMouseDelta();

private:
	//float f_lastMouseX;
	//float f_lastMouseY;
	float f_Sensitivity = 0.3f;
	bool b_isTracking;
};

#endif