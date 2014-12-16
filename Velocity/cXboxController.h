#ifndef _CXBOXCONTROLLER_H_
#define _CXBOXCONTROLLER_H_

#include <Windows.h>
#include <Xinput.h>

//A class to represent an xInput compatible controller, and retrieve button presses.
//Code paraphrased from http://www.codeproject.com/Articles/26949/Xbox-Controller-Input-in-C-with-XInput

class cXboxController
{
public:
	cXboxController(int playerNum);
	XINPUT_STATE GetState();
	bool GetControllerConnected();
	void Vibrate(int left, int right);

	void Update();

	bool GetButton(int button); //In order: A, B, X, Y, Left, Right
	bool GetButtonDown(int button); //Will return true once per button press

private: 
	XINPUT_STATE xs_controllerState;
	int i_controllerNum;

	bool button_A = false;
	bool button_B = false;
	bool button_X = false;
	bool button_Y = false;
	bool button_Left = false;
	bool button_Right = false;

	bool button_A_Latch = false;  //"Latch" values used to determine if the button has been pressed since the last execution or not
	bool button_B_Latch = false;
	bool button_X_Latch = false;
	bool button_Y_Latch = false;
	bool button_Left_Latch = false;
	bool button_Right_Latch = false;
};

#endif