//-----------------------------------------------------------------
// AbstractGame Object
// C++ Header - AbstractGame.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//
// AbstractGame is the abstract class which defines the functions that a 
// game class can implement for use in the game engine
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include <tchar.h>
#include "MainEngine.h"

//math
#include "Matrix2D.h"

//forward declarations
class Image;
class TextFormat;
class CollisionMesh;
class GameConfigurator;
//-----------------------------------------------------------------
// AbstractGame Class
//-----------------------------------------------------------------
class AbstractGame
{
public : 	
	AbstractGame() 
	{
		// nothing to create
	}

	virtual ~AbstractGame() 
	{
		// nothing to destroy
	}

	virtual void GameInitialize(GameConfigurator *gameConfiguratorPtr){}
	virtual void GameStart(ResourceLoader* resourceLoaderPtr){}
	virtual void GameEnd(){}
	virtual void GameActivate() {}																// empty definition
	virtual void GameDeactivate() {}															// empty definition
	virtual void KeyUp(TCHAR cKey){}
	virtual void GamePaint(Painter *painterPtr) {}										// empty definition
	virtual void GameTick(InputState *inputStatePtr) {}											// empty definition
	virtual void WindowResized(double width, double height) {}											// empty definition

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	AbstractGame(const AbstractGame& agRef);
	AbstractGame& operator=(const AbstractGame& agRef);
};