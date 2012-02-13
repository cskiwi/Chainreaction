//-----------------------------------------------------------------
// InputStateManager Object
// C++ Source - InputStateManager.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "InputStateManager.h"
#include "InputState.h"

//---------------------------
// Defines
//---------------------------


//---------------------------
// Constructor & Destructor
//---------------------------
InputStateManager::InputStateManager()
	:m_bIsLeftMouseButtonClicked(false)
	,m_bIsRightMouseButtonClicked(false)
	,m_bIsMiddleMouseButtonClicked(false)
	,m_bIsLeftMouseButtonDown(false)
	,m_bIsRightMouseButtonDown(false)
	,m_bIsMiddleMouseButtonDown(false)
	,m_WheelPos(0)
	,m_RenderTargetWidth(0)
	,m_RenderTargetHeight(0)
	,m_DeltaTime(0)
	,m_InputStatePtr(0)
{
	m_OldMousePos.x=0;
	m_OldMousePos.y=0;
	m_MousePos = m_OldMousePos;
	m_InputStatePtr = new InputState(
		false,false,false
		,0
		,Vector2D(0,0)
		,Point2D(0,0)
		,m_Keys
		,0,0,0);

	GetKeyboardState(m_Keys);
}

InputStateManager::~InputStateManager()
{
	delete m_InputStatePtr;
}

//---------------------------
// Eigen methoden
//---------------------------
InputState* InputStateManager::GenerateInputState(const Matrix2D& worldTransform, double deltaTime)
{
	GetKeyboardState(m_Keys);

	//calc changes since last visit
	Vector2D mouseDeltaPos = m_MousePos - m_OldMousePos;
	m_OldMousePos = m_MousePos;

	//create state object on stack
	InputState inputState(
		m_bIsLeftMouseButtonClicked, m_bIsRightMouseButtonClicked, m_bIsMiddleMouseButtonClicked,
		m_WheelPos, mouseDeltaPos/worldTransform, m_MousePos/worldTransform, 
		m_Keys, deltaTime, 
		m_RenderTargetWidth, m_RenderTargetHeight);

	*m_InputStatePtr = inputState;

	//reset data
	m_WheelPos =0;
	m_bIsLeftMouseButtonClicked = false;
	m_bIsRightMouseButtonClicked = false;
	m_bIsMiddleMouseButtonClicked = false;

	//return copy of input state object
	return m_InputStatePtr;
}

void InputStateManager::SetMousePos(double x, double y)
{
	m_MousePos.x=x;
	m_MousePos.y=y;
}

void InputStateManager::SetLeftMouseButtonState(bool isPressed)
{
	m_bIsLeftMouseButtonDown = isPressed;
	//true when button down, set to false after state has been created. result is: true only once 
	if(isPressed==false) m_bIsLeftMouseButtonClicked = true;
}

void InputStateManager::SetRightMouseButtonState(bool isPressed)
{
	m_bIsRightMouseButtonDown = isPressed;
	if(isPressed==false) m_bIsRightMouseButtonClicked = true;
}

void InputStateManager::SetMiddleMouseButtonState(bool isPressed)
{
	m_bIsMiddleMouseButtonDown = isPressed;
	if(isPressed) m_bIsMiddleMouseButtonClicked = isPressed;
}

void InputStateManager::SetWheelPos(short newWheelPos)
{
	m_WheelPos = newWheelPos;
}

bool InputStateManager::IsKeyDown( int vKey ) const
{
	if(m_Keys[vKey] & 0x80)return true;
	return false;
}

