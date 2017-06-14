#include <windows.h>
#include <XInput.h>
#include <math.h>

class Controller;


extern Controller *controller;


class Controller
{
public:
	Controller();
	~Controller();

	int GetPort();

	XINPUT_GAMEPAD * GetState();

	bool checkConnection();

	bool Update();

	bool isPressed(WORD);

	void Vibrate(int leftVal, int rightVal);


	int cId;
	XINPUT_STATE state;

	float deadzoneX, deadzoneY;
	float leftX, leftY;
	float rightX, rightY;

	bool isConnected;
};
