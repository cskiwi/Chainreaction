//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PlayField.h"

#include "Painter.h"
#include "ResourceLoader.h"
#include "InputState.h"
#include "CollisionMesh.h"
#include "Image.h"
#include "TextFormat.h"
#include "Audio.h"
#include "GameTimer.h"

//---------------------------
// Defines
//---------------------------
//

//---------------------------
// Constructor & Destructor
//---------------------------
PlayField::PlayField(int Width, int Height, int WindowWidth, int WindowHeight):
m_iWidth(Width),
	m_iHeight(Height),
	m_iWindowWidth(WindowWidth),
	m_iWindowHeight(WindowHeight)
{

}
PlayField::~PlayField()
{

}
void PlayField::Paint(Painter *PainterPtr){
	// Teken een achtergrond
	PainterPtr->Clear(51,51,51);

	// Calculating Horizontal start
	double YStart = m_iWindowWidth/2 - m_iWidth/2;
	// calculating Vertical start
	double XStart = m_iWindowHeight/2 - m_iHeight/2;

	PainterPtr->SetColor(61,61,61);
	for (int j = 25; j < m_iWidth; j +=25){
		PainterPtr->DrawLine(0 + YStart, j + XStart, m_iWidth + YStart, j + XStart,0.5);
	}

	for (int i = 25; i < m_iHeight; i +=25){
		PainterPtr->DrawLine(i + YStart ,0 + XStart, i + YStart, m_iHeight + XStart,0.5);
	}
	// Kader er rond tekenen
	PainterPtr->SetColor(255,255,255);
	PainterPtr->DrawRect(YStart,XStart,m_iWidth,m_iHeight);
}

bool PlayField::LocationInField(Point2D pos){
	double YStart = m_iWindowWidth/2 - m_iWidth/2;
	double XStart = m_iWindowHeight/2 - m_iHeight/2;

	// if in playfield
	if (pos.x >= XStart && pos.y >= YStart && pos.x <= XStart + m_iWidth && pos.y <= YStart + m_iHeight)
		return true;
	else
		return false;
}