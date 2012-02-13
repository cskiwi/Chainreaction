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
// Ball Class									
//-----------------------------------------------------
class Ball
{
public:
	Ball(ResourceLoader* ResourceLoaderPtr);				// Constructor
	virtual ~Ball();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	void Paint(Painter *PainterPtr, TextFormat *TextFormatPtr);
	void Tick(InputState *InputStatePtr, PlayField *PlayfieldPtr);

	//------------------------------------------------
	// Getter / setters
	//------------------------------------------------
	CollisionMesh * GetCollisionMesh(){return m_CollisionMeshPtr;}
	void Explode(int score);
	int GetState(){return m_iState;}

	void SetState(int state){m_iState = state;}
	void Reset();
private: 
	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_iRadius;
	int m_iExplodeScore;
	CollisionMesh* m_CollisionMeshPtr;
	Matrix2D m_matWorld;
	Vector2D m_vVelocity;
	Point2D m_pPosition;

	ColorRGBA m_ColorBall;

	int m_iState;
	static const int NORMAL = 0;
	static const int HIT = 1;
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
	Ball(const Ball& yRef);									
	Ball& operator=(const Ball& yRef);	
};


