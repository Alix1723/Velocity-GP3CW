#include "cMouseControl.h"

//Tracks mouse input (the difference in position between calls)

cMouseControl::cMouseControl()
{
	//Constructor
};

cMouseControl::~cMouseControl()
{
	//Destructor
}

void cMouseControl::StartTrackingMouse()
{
	//Start tracking the mouse and prevent it from escaping the window
	b_isTracking = true;
	SetCursorPos(512, 512); //Reset mouse to a central point
	//ShowCursor(false); //Hide cursor
}

void cMouseControl::StopTrackingMouse()
{
	b_isTracking = false;
	//ShowCursor(true); //Unhide cursor
}

bool cMouseControl::IsTracking()
{
	return b_isTracking;
}

//Call this to get the mouse movement since the last frame
std::vector<float> cMouseControl::GetMouseDelta()
{
	std::vector<float> output;
	output.push_back(0);
	output.push_back(0);

	if (b_isTracking)
	{
		//Using the GetCursorPos API
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);

		int x = (currentMousePos.x - 512);
		int y = (currentMousePos.y - 512);

		SetCursorPos(512, 512); //Reset mouse to a central point each time

		//Output x and y delta
		output[0] = (float)x * f_Sensitivity;
		output[1] = (float)y * f_Sensitivity;
	}

	return output;
}

