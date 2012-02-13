//-----------------------------------------------------------------
// Image Object
// C++ Source - Image.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#pragma once
//WIC
#include <dwrite.h>
#include <wincodec.h>
#include <math.h>

#include "D2D1.h"
class ResourceLoader;

class Image
{
public:
	Image(const String& fileName, ID2D1Bitmap *bitmapPtr, IWICFormatConverter *convertorPtr);// Constructor
	Image(int resourceID, ID2D1Bitmap *bitmapPtr, IWICFormatConverter *convertorPtr);// Constructor
	//Image(ResourceLoader* resourceLoaderPtr, const String& fileName);// Constructor
	//Image(ResourceLoader* resourceLoaderPtr, int resourceID);// Constructor
	virtual ~Image();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	//getters
	ID2D1Bitmap *GetBitmapPtr()const{ return m_BitmapPtr;}
	int GetWidth()const{ return m_BitmapPtr->GetPixelSize().width;}
	int GetHeight()const{ return m_BitmapPtr->GetPixelSize().height;}
	float GetOpacity()const{return m_Opacity;}
	IWICFormatConverter *GetFormatConverter()const{return m_ConvertorPtr;}
	//The opacity level of an image has a range from 0 to 1
	//The painter uses the opacity level to blend the image with the background
	void SetOpacity(float opacity){m_Opacity=opacity;}

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	ID2D1Bitmap *m_BitmapPtr;
	IWICFormatConverter *m_ConvertorPtr;
	float m_Opacity;//range: between 0 and 1
	String m_FileName;
	int m_ResourceID;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Image(const Image& yRef);									
	Image& operator=(const Image& yRef);	
};