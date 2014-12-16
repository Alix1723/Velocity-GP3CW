#include "cXboxController.h"

// Code paraphrased from http://www.codeproject.com/Articles/26949/Xbox-Controller-Input-in-C-with-XInput

cXboxController::cXboxController(int playerNum)
{
	i_controllerNum = playerNum;
}

XINPUT_STATE cXboxController::GetState()
{
	ZeroMemory(&xs_controllerState, sizeof(XINPUT_STATE));
	XInputGetState(i_controllerNum, &xs_controllerState);

	return xs_controllerState;
}

bool cXboxController::GetControllerConnected()
{
	ZeroMemory(&xs_controllerState, sizeof(XINPUT_STATE));
	return XInputGetState(i_controllerNum, &xs_controllerState) == ERROR_SUCCESS; //Successful error? wat
}

void cXboxController::Vibrate(int left, int right)
{
	XINPUT_VIBRATION Vibration;
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
	Vibration.wLeftMotorSpeed = left;
	Vibration.wRightMotorSpeed = right;

	XInputSetState(i_controllerNum, &Vibration);

	b_isVibrating = ((left + right) > 0);
}

//Update boolean values for inputs
void cXboxController::Update(float elapsedTime)
{
	XINPUT_STATE state = GetState();
	button_A = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
	button_B = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
	button_X = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
	button_Y = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
	
	//Note: Joystick has a maximum X range of -32767 / 32767 
	float lx = (float)((state.Gamepad.sThumbLX) / 32767);
	button_Left = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) | (lx < -0.6f);
	button_Right = (bool)(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) | (lx > 0.6f);

	if (!button_A & button_A_Latch) { button_A_Latch = false; } //Resetting latches for the next button press
	if (!button_B & button_B_Latch) { button_B_Latch = false; }
	if (!button_X & button_X_Latch) { button_X_Latch = false; }
	if (!button_Y & button_Y_Latch) { button_Y_Latch = false; }
	if (!button_Left & button_Left_Latch) { button_Left_Latch = false; }
	if (!button_Right & button_Right_Latch) { button_Right_Latch = false; }

	if (b_isVibrating) //Limit vibration time (The XBOne controller does this automatically, but the 360 doesn't)
	{
		if (f_currentVibrationTime < f_maxVibrationTime)
		{
			f_currentVibrationTime += elapsedTime;
		}
		else
		{
			b_isVibrating = false;
			Vibrate(0, 0);
			f_currentVibrationTime = 0.0f;
		}
	}
}

//Constant presses (held buttons)
bool cXboxController::GetButton(int button)
{
	if (GetControllerConnected())
	{
		switch (button)
		{
		case 1: //A
			return button_A;
		case 2: //B
			return button_B;
		case 3: //X
			return button_X;
		case 4: //Y
			return button_Y;
		case 5: //Left
			return button_Left;
		case 6: //Right
			return button_Right;
		default:
			return false;
		}
	}
	else
	{
		return false;
	}
}

//Once-only presses
bool cXboxController::GetButtonDown(int button)
{
	if (GetControllerConnected())
	{
		switch (button)
		{
		case 1: //A
			if ((button_A) & (!button_A_Latch))
				{
					button_A_Latch = true; // Only return true once per press
					return true; 
					//Note: This means calling getButtonDown() more than once in one execution will return true for the first call and false for any others after it.
				}
			else
			{
				return false;
			}
		case 2: //B
			if ((button_B) & (!button_B_Latch))
			{
				button_B_Latch = true;
				return true;
			}
			else
			{
				return false;
			}
		case 3: //X
			if ((button_X) & (!button_X_Latch))
			{
				button_X_Latch = true;
				return true;
			}
			else
			{
				return false;
			}
		case 4: //Y
			if ((button_Y) & (!button_Y_Latch))
			{
				button_Y_Latch = true;
				return true;
			}
			else
			{
				return false;
			}
		case 5: //Left
			if ((button_Left)& (!button_Left_Latch))
			{
				button_Left_Latch = true;
				return true;
			}
			else
			{
				return false;
			}
		case 6: //Right
			if ((button_Right)& (!button_Right_Latch))
			{
				button_Right_Latch = true;
				return true;
			}
			else
			{
				return false;
			}
		default:
			return false;
		}
	}
	else
	{
		return false;
	}
}