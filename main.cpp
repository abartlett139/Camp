#include "MYCLIENT.h"

MyClient *client = new MyClient();

#include <windows.h>
#include <windowsx.h>
#include <chrono>

#include "MainMenu.h"
#include "Instructions.h"
#include "Options.h"
#include "Backstory.h"
#include "Credits.h"
#include "Win.h"
#include "Lose.h"
#include "OnePlayer.h"
#include "TwoPlayer.h"


//	external Device
IDirect3DDevice9 *Device = 0;

//	external sound engine
Sound *SoundEngine = new Sound();

//	external controller
Controller *controller = new Controller();

//	external game states
GameState *currentState = 0;
GameState *previousState = 0;
GameState *menu = new MainMenu();
GameState *instructions = new Instructions();
GameState *options = new Options();
GameState *backstory = new Backstory();
GameState *credits = new Credits();
GameState *win = new Win();
GameState *lose = new Lose();
GameState *onePlayer = new OnePlayer();
GameState *twoPlayer = new TwoPlayer();


//----------------------
//	  Window Proc
//----------------------
LRESULT CALLBACK D3D::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static HICON hIcon;

	switch (msg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//----------------------
//	  Main Function
//----------------------
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR cmdLine, int showCmd) {

	//	variables for timer
	std::chrono::time_point<std::chrono::system_clock> currentTime, lastTime;
	std::chrono::duration<double> deltaTime;

	//	set current state
	currentState = menu;

	//	Initialize Sound Engine
	SoundEngine->Initialize();

	//	Initialize Direct3D
	if (!D3D::InitD3D(hinstance))
		return 0;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	currentState->Enter();
	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) && !IsDialogMessage(NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			lastTime = std::chrono::system_clock::now();
		}
		else {

			currentTime = std::chrono::system_clock::now();
			deltaTime = lastTime - currentTime;

			currentState->GameLoop((float)-deltaTime.count());

			lastTime = currentTime;
			srand(time(NULL));
		}
	}



	Device->Release();

	return (int)msg.wParam;
}


//----------------------
//	  Dialog Proc
//----------------------
INT_PTR CALLBACK Options::DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		Static_SetText(GetDlgItem(hwnd, IDC_STATIC1), "Server IP Address:");
		Static_SetText(GetDlgItem(hwnd, IDC_IPADDRESS1), client->HOST_IP);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			//	get the ip address in the ip address control box and close the dialog
			Static_GetText(GetDlgItem(hwnd, IDC_IPADDRESS1), client->HOST_IP, sizeof(client->HOST_IP) + 1);
			EndDialog(hwnd, 0);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return FALSE;
}