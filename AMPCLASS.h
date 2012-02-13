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
// AMPCLASS Class									
//-----------------------------------------------------
class AMPCLASS
{
public:
	AMPCLASS();				// Constructor
	virtual ~AMPCLASS();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------


private: 
	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------


	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	AMPCLASS(const AMPCLASS& yRef);									
	AMPCLASS& operator=(const AMPCLASS& yRef);	
};

 
