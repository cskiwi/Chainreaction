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

//-----------------------------------------------------
// PlayField Class									
//-----------------------------------------------------
class PlayField
{
public:
	PlayField(int Width, int Height, int WindowWidth, int WindowHeight);				// Constructor
	virtual ~PlayField();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	void Paint(Painter *PainterPtr);

	//------------------------------------------------
	// Getter / setters
	//------------------------------------------------
	int GetWidth(){return m_iWidth;}
	int GetHeight(){return m_iHeight;}
	bool LocationInField(Point2D pos);

private: 
	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_iWidth, m_iHeight;
	int m_iWindowWidth, m_iWindowHeight;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PlayField(const PlayField& yRef);									
	PlayField& operator=(const PlayField& yRef);	
};


