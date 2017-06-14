#include "GameState.h"



GameState::GameState()
{
}


GameState::~GameState()
{
}


void GameState::GameLoop(float deltaTime)
{
	//	render current state
	currentState->Render(deltaTime);

	//	check for controller connection
	if (!controller->Update()) {
		if (!controller->checkConnection())
			controller->isConnected = false;
	}
	else {
		controller->isConnected;
	}

	//	update current state
	currentState->Update(deltaTime);
}