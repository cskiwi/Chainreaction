//-----------------------------------------------------------------
// GameConfigurator Object
// C++ Header - GameConfigurator.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "resource.h"
#include "GameConfigurator.h"



//---------------------------
// Defines
//---------------------------
//

//---------------------------
// Constructor & Destructor
//---------------------------
GameConfigurator::GameConfigurator()
	:m_RendertargetWidth(640)
	,m_RendertargetHeight(480)
	,m_NumPaintsPerSec(0)
	,m_NumTicksPerSec(0)
	,m_MaxDeltaTime(0.1)
	,m_sWindowTitle("AMPEngine")
	,m_bEnableWindowScale(false)
	,m_WindowScale(1.0)
	,m_bEnableWindowResize(true)
{
	
}

GameConfigurator::~GameConfigurator()
{
	
}

//---------------------------
// Eigen methoden
//---------------------------

void GameConfigurator::SetWidth(int widthInPixels)
{
	m_RendertargetWidth=max(widthInPixels,160);
	m_RendertargetWidth=min(widthInPixels,1920);
}

void GameConfigurator::SetHeight(int heightInPixels)
{
	m_RendertargetHeight=max(heightInPixels,120);
	m_RendertargetHeight=min(heightInPixels,1080);
}

void GameConfigurator::SetNumPaintsPerSec(int numPaintsPerSec)
{
	DEVMODE devMode;
	devMode.dmSize = sizeof(devMode);
	EnumDisplaySettings(NULL, 0, &devMode);

	m_NumPaintsPerSec = max(numPaintsPerSec,0);
	m_NumPaintsPerSec = min(numPaintsPerSec,(int)devMode.dmDisplayFrequency);
}

void GameConfigurator::SetNumTicksPerSec(int numTicksPerSec)
{
	m_NumTicksPerSec = max(numTicksPerSec,0);
	m_NumTicksPerSec = min(numTicksPerSec,1000);
}

void GameConfigurator::SetWindowTitle(String windowTitle)
{
	m_sWindowTitle=windowTitle;
}

void GameConfigurator::EnableWindowScale(bool b)
{
	m_bEnableWindowScale=b;
}

bool GameConfigurator::IsWindowScaleEnabled() const
{
	return m_bEnableWindowScale;
}

void GameConfigurator::SetWindowScale(double scale)
{
	m_WindowScale = scale;
}

double GameConfigurator::GetWindowScale() const
{
	return m_WindowScale;
}