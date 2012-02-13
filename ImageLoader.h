//-----------------------------------------------------------------
// ImageLoader Object
// C++ Source - ImageLoader.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#pragma once
//WIC
#include <dwrite.h>
#include <wincodec.h>
#include <math.h>

#include "D2D1.h"

class Image;

class ImageLoader
{
public:
	//ImageLoader();// Constructor
	ImageLoader(ID2D1RenderTarget *renderTargetPtr, IWICImagingFactory *wICFactoryPtr);// Constructor
	virtual ~ImageLoader();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	Image* LoadImage(const String& fileName);
//	bool LoadImage(const String& fileName, ID2D1Bitmap **ppbitmap, IWICFormatConverter **ppConvertor);
	Image* LoadImage(int resourceID);
	
private:
	//*loads an image from a resource number
	HRESULT LoadResourceBitmap( ID2D1RenderTarget *renderTargetPtr, IWICImagingFactory *wICFactoryPtr, unsigned int resourceNumber, PCWSTR resourceType, IWICFormatConverter **formatConvertorPtrPtr);
	//*loads an image from a resource name
	HRESULT LoadResourceBitmap( ID2D1RenderTarget *renderTargetPtr, IWICImagingFactory *wICFactoryPtr, PCWSTR resourceName, PCWSTR resourceType, IWICFormatConverter **formatConvertorPtrPtr);
	//* loads an image from a filename
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget *renderTargetPtr, IWICImagingFactory *wICFactoryPtr, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, IWICFormatConverter **formatConvertorPtrPtr);
	//Gets WIC factory
	IWICImagingFactory *GetWICFactory()const{return m_WICFactoryPtr;}

private: 
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	ID2D1RenderTarget *m_RenderTargetPtr;
	// WIC and Bitmaps
    IWICImagingFactory *m_WICFactoryPtr;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ImageLoader(const ImageLoader& yRef);									
	ImageLoader& operator=(const ImageLoader& yRef);	
};