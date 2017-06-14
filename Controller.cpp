#include "Controller.h"



Controller::Controller()
{
	deadzoneX = 0.25f;
	deadzoneY = 0.25f;
}


Controller::~Controller()
{
}


int Controller::GetPort()
{

	return cId + 1;
}

XINPUT_GAMEPAD * Controller::GetState()
{
	return &state.Gamepad;
}

bool Controller::checkConnection()
{
	int controllerId = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			controllerId = i;
	}

	cId = controllerId;


	return controllerId != -1;
}

//if controller is unpluged or disconnected
bool Controller::Update()
{
	if (cId == -1)
		checkConnection();

	if (cId != -1)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(cId, &state) != ERROR_SUCCESS)
		{
			cId = -1;
			return false;
		}

		//check for input and make deadzones
		float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

		leftX = (fabs(normLX) < deadzoneX ? 0 : (fabs(normLX) - deadzoneX) * (normLX / fabs(normLX)));
		leftY = (fabs(normLY) < deadzoneY ? 0 : (fabs(normLY) - deadzoneY) * (normLY / fabs(normLY)));

		if (deadzoneX > 0) leftX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) leftY *= 1 / (1 - deadzoneY);

		float normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);

		rightX = (fabs(normRX) < deadzoneX ? 0 : (fabs(normRX) - deadzoneX) * (normRX / fabs(normRX)));
		rightY = (fabs(normRY) < deadzoneY ? 0 : (fabs(normRY) - deadzoneY) * (normRY / fabs(normRY)));

		if (deadzoneX > 0) rightX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) rightY *= 1 / (1 - deadzoneY);

		return true;
	}

	return false;
}

bool Controller::isPressed(WORD button)
{
	return (state.Gamepad.wButtons & button) != 0;
}

void Controller::Vibrate(int leftVal, int rightVal)
{
	// create vibration state
	XINPUT_VIBRATION Vibration;

	// clear vibration struct
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(cId, &Vibration);


}