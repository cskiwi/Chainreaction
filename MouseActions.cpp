//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "resource.h"
#include "MouseActions.h"

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
MouseActions::MouseActions(ResourceLoader* ResourceLoaderPtr):
m_iRadius(50),
	m_iBigRadius(50),
	m_CollisionMeshPtr(0),
	m_iState(NORMAL),
	m_dTimeBig(0)
{
	m_CollisionMeshPtr = ResourceLoaderPtr->GetCollisionMesh();
	m_CollisionMeshPtr->CreateEllipse(0,0,m_iRadius, m_iRadius);
}

MouseActions::~MouseActions()
{
	delete m_CollisionMeshPtr;
	m_CollisionMeshPtr = 0;
}

void MouseActions::Tick(InputState* InputStatePtr, PlayField* PlayfieldPtr){
	// Setting / creating value's
	double Dtime = InputStatePtr->GetDeltaTime();
	double ExpandRate;

	// Switch States
	switch(m_iState){
	case NORMAL:
		// if in playfield
		if (PlayfieldPtr->LocationInField(InputStatePtr->GetMousePos())){
				m_matWorld.SetAsTranslate(InputStatePtr->GetMousePos());
		}
		break;
	case CLICKED:
		ExpandRate = 0.1 / Dtime;
		m_iRadius < BIG_RADIUS ? m_iRadius += NORMAL_RADIUS/ExpandRate : m_iState = BIG;
		m_CollisionMeshPtr->CreateEllipse(0,0,m_iRadius,m_iRadius);
		break;
	case TIMEUP:
		ExpandRate = 0.1 / Dtime;
		m_iRadius >= 0 ? m_iRadius -= NORMAL_RADIUS/ExpandRate : m_iState = GONE;
		m_CollisionMeshPtr->CreateEllipse(0,0,m_iRadius,m_iRadius);
		break;
	case BIG:
		m_dTimeBig += Dtime;
		if (m_dTimeBig > 1)
			m_iState = TIMEUP;
		break;
	}
	m_CollisionMeshPtr->SetTransformMatrix(m_matWorld);
}

void MouseActions::Paint(Painter* PainterPtr){
	PainterPtr->SetTransformMatrix(m_matWorld);

	//paint the elipse
	PainterPtr->SetColor(255,255,255,100);
	if (m_iState != GONE)
		PainterPtr->FillEllipse(0,0,m_iRadius, m_iRadius);

	// reset world
	PainterPtr->SetTransformMatrix(Matrix2D::Identity());
	// reset the color
	PainterPtr->SetColor(255,255,255);

	// collision mesh
	//PainterPtr->SetColor(255,0,0,130);
	// PainterPtr->FillCollisionMesh(m_CollisionMeshPtr);

	PainterPtr->SetColor(255,255,255);
}
void MouseActions::Reset(){
	m_iRadius = 50;
	m_iBigRadius = 50;
	m_iState = NORMAL;
	m_dTimeBig = 0;
}