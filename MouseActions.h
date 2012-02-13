#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Matrix2D.h"

//-----------------------------------------------------
// Class Forward Declarations
//-----------------------------------------------------
class ResourceLoader;
class Painter;
class InputState;
class Image;
class CollisionMesh;
class TextFormat;
class Audio;

class PlayField;

//-----------------------------------------------------
// MouseActions Class									
//-----------------------------------------------------
class MouseActions
{
public:
	MouseActions(ResourceLoader* ResourceLoaderPtr);				// Constructor
	virtual ~MouseActions();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	void Paint(Painter* PainterPtr);
	void Tick(InputState* InputStatePtr, PlayField* PlayfieldPtr);

	//------------------------------------------------
	// Getter / setters
	//------------------------------------------------
	void SetPosition(Point2D Position);
	void SetClicked(){if(m_iState == NORMAL){ m_iState = CLICKED; m_iRadius = 0;}}
	int GetState(){return m_iState;}
	void Reset();

	CollisionMesh * GetCollisionMesh(){return m_CollisionMeshPtr;}

private: 
	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_iRadius;
	int m_iBigRadius;
	CollisionMesh *m_CollisionMeshPtr;
	Matrix2D m_matWorld;

	int m_iState;
	static const int NORMAL = 0;
	static const int CLICKED = 1;
	static const int BIG = 2;
	static const int TIMEUP = 3;
	static const int GONE = 4;

	double m_dTimeBig;

	// Defaults:
	static const int NORMAL_RADIUS = 10;
	static const int BIG_RADIUS = 50;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MouseActions(const MouseActions& yRef);									
	MouseActions& operator=(const MouseActions& yRef);	
};


