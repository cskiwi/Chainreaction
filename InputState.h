//-----------------------------------------------------------------
// InputState Object
// C++ Source - InputState.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#pragma once
#include "Matrix2D.h"

class InputState
{
public:
	InputState(
		bool bIsLeftMouseButtonClicked, bool bIsRightMouseButtonClicked, bool bIsMiddleMouseButtonClicked, 
		int iWheelPos, Vector2D mouseDelta, Point2D mousePos,
		unsigned char *keys, double dTime,
		double renderTargetWidth,	double renderTargetHeight);

	virtual ~InputState(void);
	
	//void Clone(InputState &inputStateRef);

	// these getters return the state of mouse and keyboard
	// the status of input devices is updated once per frame 
	// in the UpdateScene method calling GetInputState()

	//IsKeyDown: returns true if key vKey is down
	//Example: inputStatePtr->IsKeyDown(VK_UP);
	//Example: inputStatePtr->IsKeyDown('A')
	bool IsKeyDown(int vKey) const;

	//GetMouseMovement: returns a Point2D containing the difference in mousepos since last GameTick()
	Vector2D GetMouseDisplacement() const;

	//GetMousePos: returns the position of the mouse in device independent space
	Point2D GetMousePos() const;

	//Returns nonzero when wheel is turned
	short GetWheelPos() const;

	//Returns true if the left mouse button is released
	bool IsLeftMouseButtonClicked() const{return m_bIsLeftMouseButtonClicked;}
	
	//Returns true if the right mouse button is released
	bool IsRightMouseButtonClicked() const{return m_bIsRightMouseButtonClicked;}

	bool IsMiddleMouseButtonClicked() const { return m_bIsMiddleMouseButtonClicked; }

	//Returns true if the left mousebutton is pressed 
	//Returns false if the left mousebutton is not pressed
	bool IsLeftMouseButtonDown()const ;

	//Returns true if the right mousebutton is pressed
	//Returns false if the right mousebutton is not pressed
	bool IsRightMouseButtonDown()const ;

	bool IsMiddleMouseButtonDown() const;

	//GetDeltaTime: returns the time in seconds that has passed sicne last call to GameTick()
	double GetDeltaTime() const{return m_DeltaTime;}
	void SetDeltaTime(double deltaTime) { m_DeltaTime = deltaTime; }

	Rect2D GetWindowBounds() const { return Rect2D(0,0,m_RenderTargetWidth,m_RenderTargetHeight); }

	//Returns the width of the rendertarget (window client area)
	double GetWindowWidth()const { return m_RenderTargetWidth; }

	//Returns the height of the rendertarget (window client area)
	double GetWindowHeight() const { return m_RenderTargetHeight; }

	const Matrix2D& GetWorldTransform() const { return m_WorldTransform; }

	// Pause the game engine.
	// Resume by pressing the PAUSE key.
	void Pause() { m_bIsPaused = true; }
	bool IsPaused() const { return m_bIsPaused; }

    void Repaint() { m_bIsDirty = true; }
    bool IsDirty() const { return m_bIsDirty; }

	//Constants
	static const int NUMKEYS=256;

protected:
	bool m_bIsLeftMouseButtonClicked;
	bool m_bIsRightMouseButtonClicked;
	bool m_bIsMiddleMouseButtonClicked;

    bool m_bIsPaused;
    bool m_bIsDirty;

	short m_WheelPos;
	Point2D m_MousePos;
	Vector2D m_MouseDisplacement;
	Matrix2D m_WorldTransform;

	unsigned char m_Keys[NUMKEYS];

	double m_DeltaTime;
	double m_RenderTargetWidth;
	double m_RenderTargetHeight;
	
};
