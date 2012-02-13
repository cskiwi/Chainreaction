//-----------------------------------------------------------------
// Game Engine WinMain Function
// C++ Source - GameWinMain.cpp - version 2011 v1_30
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#pragma once
#include "StdAfx.h"
//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GameWinMain.h"

#include "Chainreaction.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Entry Point Functions
//-----------------------------------------------------------------
//used if console subsystem
void main()
{
	WinMain(0,0,0,0);
}
//used if windows subsystem
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL,0);

	// Enable run-time memory leak check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	MainEngine engine;
	//AbstractGame *gamePtr = new Test();
	//AbstractGame *gamePtr = new RType();
	//AbstractGame *gamePtr = new BouncingBall();
	//AbstractGame *gamePtr = new BouncingBalls();
	//AbstractGame *gamePtr = new Goniometry();
	AbstractGame *gamePtr = new Chainreaction();
	//AbstractGame *gamePtr = new SolarSystem();
	//AbstractGame *gamePtr = new ButtonTextBoxTestApp();
	//AbstractGame *gamePtr = new LineCollision();
	//AbstractGame *gamePtr = new FollowMouse();
	//AbstractGame *gamePtr = new PolygonTester();
	//AbstractGame *gamePtr = new AlteredBeast();
	//AbstractGame *gamePtr = new CaveApp();
	//AbstractGame *gamePtr = new AudioTester();

	engine.SetGame(gamePtr);
	if (SUCCEEDED(engine.Initialize()))
	{
		engine.RunMessageLoop();
	}
	delete gamePtr;
	return 0;
}