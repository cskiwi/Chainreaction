//-----------------------------------------------------------------
// InputState Object
// C++ Source - InputState.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#include "StdAfx.h"
#include "InputState.h"


InputState::InputState(
	bool bIsLeftMouseButtonClicked, 
	bool bIsRightMouseButtonClicked, 
	bool bIsMiddleMouseButtonClicked, 
	int iWheelPos, 
	Vector2D mouseDelta, 
	Point2D mousePos,
	byte *pKeys,
	double dTime,
	double renderTargetWidth,
	double renderTargetHeight)
	:m_bIsLeftMouseButtonClicked(bIsLeftMouseButtonClicked)
	,m_bIsRightMouseButtonClicked(bIsRightMouseButtonClicked)
	,m_bIsMiddleMouseButtonClicked(bIsMiddleMouseButtonClicked)
	,m_WheelPos(iWheelPos)
	,m_MouseDisplacement(mouseDelta)
	,m_MousePos(mousePos)
	,m_DeltaTime(dTime)
	,m_RenderTargetWidth(renderTargetWidth)
	,m_RenderTargetHeight(renderTargetHeight)
	,m_bIsPaused(false)
    ,m_bIsDirty(false)
{
	memcpy_s(m_Keys, NUMKEYS, pKeys, NUMKEYS);
	if( IsKeyDown(VK_MENU) )
	{
		// When ALT is pressed, the engine handles mouse moves.
		m_Keys[VK_LBUTTON] = false;
		m_Keys[VK_RBUTTON] = false;
		m_Keys[VK_MBUTTON] = false;
		m_MouseDisplacement = Vector2D(0,0);
	}
}

InputState::~InputState(void)
{

}

//void InputState::Clone(InputState &inputStateRef)
//{
//	m_bLMBClicked = inputStateRef.m_bLMBClicked;
//	m_bRMBClicked = inputStateRef.m_bRMBClicked;
//	m_bLMBDown = inputStateRef.m_bLMBDown;
//	m_bRMBDown = inputStateRef.m_bRMBDown;
//	m_WheelPos = inputStateRef.m_WheelPos;
//	m_MouseDeltaPos = inputStateRef.m_MouseDeltaPos;
//	m_MousePos = inputStateRef. m_MousePos;
//	m_DTime = inputStateRef.m_DTime;
//	m_RenderTargetWidth = inputStateRef.m_RenderTargetWidth;
//	m_RenderTargetHeight = inputStateRef.m_RenderTargetHeight;
//	memcpy_s(m_Keys, NUMKEYS, inputStateRef.m_Keys, NUMKEYS);
//}

bool InputState::IsKeyDown(int vKey) const
{
	if(m_Keys[vKey] & 0x80)return true;
	return false;
}

Vector2D InputState::GetMouseDisplacement() const
{
	return m_MouseDisplacement;
}

Point2D InputState::GetMousePos() const
{
	return m_MousePos;
}

short  InputState::GetWheelPos() const
{
	return m_WheelPos;
}

bool InputState::IsLeftMouseButtonDown() const
{
   return IsKeyDown(VK_LBUTTON);
}

bool InputState::IsRightMouseButtonDown() const
{
   return IsKeyDown(VK_RBUTTON);
}

bool InputState::IsMiddleMouseButtonDown() const
{
   return IsKeyDown(VK_MBUTTON);
}
