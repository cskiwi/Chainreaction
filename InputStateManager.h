//-----------------------------------------------------------------
// InputStateManager Object
// C++ Source - InputStateManager.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "InputState.h"
#include "GameTimer.h"
#include "Matrix2D.h"
//-----------------------------------------------------
// InputStateManager Class									
//-----------------------------------------------------
class InputStateManager
{
public:
	InputStateManager();				// Constructor
	virtual ~InputStateManager();		// Destructor


	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	// next setters are called by windows message loop proc
	void SetMousePos(double x, double y);

	void SetLeftMouseButtonState(bool isPressed);
	void SetRightMouseButtonState(bool isPressed);
	void SetMiddleMouseButtonState(bool isPressed);

	void SetWheelPos(short zDelta);
	void SetWidth(double width){m_RenderTargetWidth = width;}
	void SetHeight(double height){m_RenderTargetHeight = height;}

	// reads the status of keyboard && calcs the changed mouse coords
	InputState* GenerateInputState(const Matrix2D& worldTransform, double deltaTime);

	bool IsKeyDown(int vKey) const;

	Point2D GetMousePos() const { return m_MousePos; }
	short  GetWheelPos() const { return m_WheelPos;}

	bool IsLeftMouseButtonDown() const { return IsKeyDown(VK_LBUTTON); }
	bool IsRightMouseButtonDown() const { return IsKeyDown(VK_RBUTTON); }
	bool IsMiddleMouseButtonDown() const { return IsKeyDown(VK_MBUTTON); }

	bool IsLeftMouseButtonClicked() const { return m_bIsLeftMouseButtonClicked; }
	bool IsRightMouseButtonClicked() const { return m_bIsRightMouseButtonClicked; }
	bool IsMiddleMouseButtonClicked() const { return m_bIsMiddleMouseButtonClicked; }

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	Point2D m_OldMousePos;
	Point2D m_MousePos;

	double m_DeltaTime;

	unsigned char m_Keys[256];

	bool m_bIsLeftMouseButtonClicked;
	bool m_bIsRightMouseButtonClicked;
	bool m_bIsMiddleMouseButtonClicked;

	bool m_bIsLeftMouseButtonDown;
	bool m_bIsRightMouseButtonDown;
	bool m_bIsMiddleMouseButtonDown;

	short m_WheelPos;
	double m_RenderTargetWidth, m_RenderTargetHeight;

	//this pointer is used to transport to the game
	InputState *m_InputStatePtr;

	// Disabling default copy constructor and default 
	// assignment operator.
	InputStateManager(const InputStateManager& yRef);									
	InputStateManager& operator=(const InputStateManager& yRef);	
};

 
