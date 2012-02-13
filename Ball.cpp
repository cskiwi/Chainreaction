//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "resource.h"
#include "Ball.h"

#include "Painter.h"
#include "ResourceLoader.h"
#include "InputState.h"
#include "CollisionMesh.h"
#include "Image.h"
#include "TextFormat.h"
#include "Audio.h"
#include "GameTimer.h"

#include "PlayField.h"

//---------------------------
// Defines
//---------------------------
//

//---------------------------
// Constructor & Destructor
//---------------------------
Ball::Ball(ResourceLoader* ResourceLoaderPtr):
m_iRadius(10),
	m_CollisionMeshPtr(0),
	m_iState(NORMAL),
	m_vVelocity(0, 0),
	m_ColorBall(rand()%256,rand()%256,rand()%256,rand()%100+156),
	m_dTimeBig(0),
	m_iExplodeScore(0)
{
	m_CollisionMeshPtr = ResourceLoaderPtr->GetCollisionMesh();
	m_CollisionMeshPtr->CreateEllipse(0,0,m_iRadius, m_iRadius);
}

Ball::~Ball()
{
	delete m_CollisionMeshPtr;
	m_CollisionMeshPtr = 0;
}

//---------------------------
// Eigen methoden
//---------------------------

void Ball::Tick(InputState *InputStatePtr, PlayField *PlayfieldPtr){

	// Setting / creating value's
	double Dtime = InputStatePtr->GetDeltaTime();
	double WindowWidth, WindowHeight;
	double Width, Height;
	double YStart, XStart;
	double Angle;
	double ExpandRate;
	double RadiousScale;
	Matrix2D matScale, matTranslate;

	// Switch States
	switch(m_iState){
	case NORMAL:
		Width = PlayfieldPtr->GetWidth();
		Height = PlayfieldPtr->GetHeight();
		WindowWidth = InputStatePtr->GetWindowWidth();
		WindowHeight = InputStatePtr->GetWindowHeight();

		// check if Position is set
		if (m_pPosition.x == 0) m_pPosition.x = WindowWidth/2 - (rand()%((int)Width/2)-(int)Width/4);
		if (m_pPosition.y == 0) m_pPosition.y = WindowHeight/2 - (rand()%((int)Width/2)-(int)Width/4);

		if (m_vVelocity.x == 0 || m_vVelocity.y == 0){
			// creating a angle 
			Angle = rand()%360;

			// setting the x and y coordinates (value between -1 and 1) so added some speed
			m_vVelocity.x = cos(Angle) * 200;
			m_vVelocity.y = sin(Angle) * 200;
		}
		// New location over time
		m_pPosition += m_vVelocity*Dtime;

		// Calculating X and Y start coordinates
		XStart = WindowWidth/2 - Width/2;
		YStart = WindowHeight/2 - Height/2;

		// Border checking + changing direction
		if		(m_pPosition.x + m_iRadius > WindowWidth - XStart - 5		&& m_vVelocity.x > 0) m_vVelocity.x *= -1;
		else if	(m_pPosition.y + m_iRadius > WindowHeight - YStart - 5		&& m_vVelocity.y > 0) m_vVelocity.y *= -1;

		if		(m_pPosition.x - m_iRadius < XStart + 5	&& m_vVelocity.x < 0) m_vVelocity.x *= -1;
		else if	(m_pPosition.y - m_iRadius < YStart + 5	&& m_vVelocity.y < 0) m_vVelocity.y *= -1;
		break;
	case HIT:
		ExpandRate = 0.1 / Dtime;
		m_iRadius < BIG_RADIUS ? m_iRadius += NORMAL_RADIUS/ExpandRate : m_iState = BIG;
		matScale.SetAsScale(RadiousScale);
		
		// Old code with creating new colMesh
		//m_CollisionMeshPtr->CreateEllipse(0,0,m_iRadius,m_iRadius);
		break;
	case TIMEUP:
		ExpandRate = 0.1 / Dtime;
		m_iRadius >= 0 ? m_iRadius -= NORMAL_RADIUS/ExpandRate : m_iState = GONE;
		matScale.SetAsScale(m_iRadius/50);

		// Old code with creating new colMesh
		//m_CollisionMeshPtr->CreateEllipse(0,0,m_iRadius,m_iRadius);
		break;
	case BIG:
		m_dTimeBig += Dtime;
		if (m_dTimeBig > 1)
			m_iState = TIMEUP;
		break;
	}

	matTranslate.SetAsTranslate(m_pPosition);
	m_matWorld = matScale * matTranslate;
	m_CollisionMeshPtr->SetTransformMatrix(m_matWorld);
}
void Ball::Paint(Painter *PainterPtr, TextFormat *TextFormatPtr){
	String Score = String("") + m_iExplodeScore;
	int LengthText = Score.GetLength()*12;

	PainterPtr->SetTransformMatrix(m_matWorld);
	PainterPtr->SetColor(m_ColorBall);

	//paint the elipse + score if setted
	if (m_iState != GONE){
		PainterPtr->FillEllipse(0,0,m_iRadius, m_iRadius);
		PainterPtr->SetColor(255,255,255,100);
		if (m_iExplodeScore != 0)
			PainterPtr->DrawString(TextFormatPtr,Score, -LengthText/3,-6);
	}

	// Reset world
	PainterPtr->SetTransformMatrix(Matrix2D::Identity());
	// reset the color
	PainterPtr->SetColor(255,255,255);

	// Collision mesh
	//PainterPtr->SetColor(255,0,0,130);
	//PainterPtr->FillCollisionMesh(m_CollisionMeshPtr);
}
void Ball::Explode(int score){
	if (m_iState == NORMAL) m_iState = HIT;
	if (m_iExplodeScore == 0) m_iExplodeScore = score;
}

void Ball::Reset(){
	m_iRadius = NORMAL_RADIUS;
	m_iState = NORMAL;
	m_dTimeBig = 0;
	m_pPosition = Point2D();
	m_vVelocity = Vector2D();
	m_iExplodeScore = 0;
}
