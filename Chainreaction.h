#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "AbstractGame.h"

//#include embedded resources
#include "Resource.h"
//include base class header
#include "Caller.h"

class ResourceManager;
class Painter;
class InputState;
class Image;
class CollisionMesh;
class TextFormat;
class Button;
class TextBox;
class GameConfigurator;
class Audio;

class Ball;
class PlayField;
class MouseActions;
//-----------------------------------------------------------------
// Chainreaction Class																
//-----------------------------------------------------------------
class Chainreaction : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s)
	//---------------------------
	Chainreaction();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Chainreaction();

	//---------------------------
	// General Public Methods
	//---------------------------

	void GameInitialize(GameConfigurator* GameConfiguratorPtr);
	void GameStart(ResourceLoader* resourceManagerPtr);
	void KeyUp(TCHAR CKey);
	void GamePaint(Painter* PainterPtr);
	void GameTick(InputState* InputStatePtr);
	void GameEnd();
	void CallAction(Caller* CallerPtr);
	void WindowResized(double Width, double Height);										// empty definition

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void PaintHUD(Painter* PainterPtr);
	void Hittest(Ball* Ball1Ptr, Ball* BallPtr2);
	void Hittest(Ball* Ball1Ptr, MouseActions* MouseActions);
	void Reset();

	// -------------------------
	// Datamembers
	// -------------------------
	int m_iActiveBalls;
	int m_iBallsNeededForLevel;
	static const int MAX_BALLS= 50;
	Ball* m_BallArrPtr[MAX_BALLS];
	int m_iTotalScore;

	double m_dTime;
	int m_iState;
	static const int MENU = 0;
	static const int GAME = 1;

	int m_iGameResult;
	static const int NOTPLAYED = 0;
	static const int LOST = 1;
	static const int WON = 2;

	// Objects
	PlayField *m_PlayFieldPtr;
	TextFormat *m_TextformatPtr;
	MouseActions *m_MouseActionsPtr;

	// HUD
	Button* m_btnResetButtonPtr;
	TextBox* m_txtAmountOfBallsPtr;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Chainreaction(const Chainreaction& tRef);
	Chainreaction& operator=(const Chainreaction& tRef);
};
