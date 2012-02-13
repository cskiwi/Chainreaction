//-----------------------------------------------------------------
// ResourceLoader Object
// C++ Source - ResourceLoader.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
//#include "d3dUtil.h"
#include "Rect2D.h"
//-----------------------------------------------------
// Class forward declarations
//-----------------------------------------------------
class ImageLoader;
class TextFormatLoader;
class TextFormat;
class CollisionMesh;
class Image;
class TextFormat;
class Button;
class TextBox;

//-----------------------------------------------------
// ResourceLoader Class									
//-----------------------------------------------------
class ResourceLoader
{
public:
	ResourceLoader(HWND hwnd, ID2D1Factory *d2DFactoryPtr, ID2D1RenderTarget *renderTargetPtr);				// Constructor
	virtual ~ResourceLoader();		// Destructor

    // Get the rectangle of the canvas.
    Rect2D GetCanvasBounds();

    // Get the width of the canvas (rendertarget)
    // Just here for backwards compatibility
    double GetWidth() { return GetCanvasBounds().GetWidth(); }

    // Get the height of the canvas (rendertarget)
    // Just here for backwards compatibility
    double GetHeight() { return GetCanvasBounds().GetHeight(); }

	/*Loads an Image and returns the pointer to the created Image object.*/
	//Delete the object at the end of your program
	Image* LoadImage(const String& fileName);

	/*Loads an Image and returns the pointer to the created Image object.*/
	//Delete the object at the end of your program
	Image* LoadImage(int resourceID);

	/*Builds a TextFormat object and returns the pointer*/
	//Delete the object at the end of your program
	TextFormat* LoadTextFormat(const String&fontName, float size);
	
	/*Creates a CollisionMesh object and returns the pointer*/
	CollisionMesh* GetCollisionMesh();
	//not intended for students
	ID2D1Factory *GetFactory()const {return m_D2DFactoryPtr;}
	//not intended for students
	IWICImagingFactory *GetWICFactory()const {return m_WICFactoryPtr;}
	//not intended for students
	HWND GetWindowHandle()const {return m_Hwnd;}
	//CollisionMesh* GetGeometry();

	//GetButton creates a Button object and returns the pointer
	//Delete the object at the end of your program
	Button* GetButton(const String &text);

	//GetTextBox creates a TextBox object and returns the pointer
	//Delete the object at the end of your program
	TextBox * GetTextBox(const String &text);

	double GetWidth()const;
	double GetHeight()const;
private: 
	//not intended for students
	void CreateWICFactory();
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	IWICImagingFactory *m_WICFactoryPtr;
	ID2D1Factory *m_D2DFactoryPtr;
	ID2D1RenderTarget *m_RenderTargetPtr;
	ImageLoader *m_ImageLoaderPtr;
	TextFormatLoader *m_TextFormatLoaderPtr;

	HWND m_Hwnd;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ResourceLoader(const ResourceLoader& yRef);									
	ResourceLoader& operator=(const ResourceLoader& yRef);	
};

 
