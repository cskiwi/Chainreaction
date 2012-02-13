//-----------------------------------------------------------------
// ResourceLoader Object
// C++ Source - ResourceLoader.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "ResourceLoader.h"
#include "ImageLoader.h"
#include "TextFormatLoader.h"
#include "CollisionMesh.h"
#include "Button.h"
#include "TextBox.h"


//---------------------------
// Constructor & Destructor
//---------------------------
ResourceLoader::ResourceLoader(HWND hwnd, ID2D1Factory *d2DFactoryPtr, ID2D1RenderTarget *renderTargetPtr)
	:m_Hwnd(hwnd)
	,m_ImageLoaderPtr(0)
	,m_TextFormatLoaderPtr(0)
	,m_RenderTargetPtr(renderTargetPtr)

	,m_D2DFactoryPtr(d2DFactoryPtr)
	,m_WICFactoryPtr(0)

{
	CreateWICFactory();
	m_ImageLoaderPtr = new ImageLoader(renderTargetPtr, m_WICFactoryPtr);
	m_TextFormatLoaderPtr = new TextFormatLoader();
}

ResourceLoader::~ResourceLoader()
{
	delete m_ImageLoaderPtr;
	delete m_TextFormatLoaderPtr;
	//remove WICFactory?
}
void ResourceLoader::CreateWICFactory()
{
	HRESULT hr;
	// Create a WIC factory if it does not exists
	if(!m_WICFactoryPtr)
	{
		hr = CoCreateInstance(CLSID_WICImagingFactory,NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&m_WICFactoryPtr));
		if(hr!=S_OK)m_WICFactoryPtr=0;
	}
}
//pass through methods

Image* ResourceLoader::LoadImage(const String& fileName)
{
	return m_ImageLoaderPtr->LoadImage(fileName);
}
Image* ResourceLoader::LoadImage(int resourceID)
{
	return m_ImageLoaderPtr->LoadImage(resourceID);
}

TextFormat* ResourceLoader::LoadTextFormat(const String& fontName, float size)
{
	return m_TextFormatLoaderPtr->LoadTextFormat(fontName.ToTChar(), size); 
}

CollisionMesh* ResourceLoader::GetCollisionMesh()
{
	return new CollisionMesh(m_D2DFactoryPtr,m_WICFactoryPtr);
}

Button* ResourceLoader::GetButton(const String &text)
{
	return new Button(m_Hwnd, text);
}
TextBox * ResourceLoader::GetTextBox(const String &text)
{
	return new TextBox(m_Hwnd, text);

}

double ResourceLoader::GetWidth()const
{
	D2D1_SIZE_F renderTargetSize = m_RenderTargetPtr->GetSize();
	return renderTargetSize.width;
}
double ResourceLoader::GetHeight()const
{
	D2D1_SIZE_F renderTargetSize = m_RenderTargetPtr->GetSize();
	return renderTargetSize.height;

}

Rect2D ResourceLoader::GetCanvasBounds()
{
	D2D1_SIZE_F renderTargetSize = m_RenderTargetPtr->GetSize();
	Rect2D rect(0,0,renderTargetSize.width,renderTargetSize.height) ;
	return rect;
}

//bool ResourceLoader::GetImage(const String& fileName, ID2D1Bitmap **formatConvertorPtrPtr, IWICFormatConverter **ppConvertor)
//{
//	return m_ImageLoaderPtr->LoadImage(fileName, formatConvertorPtrPtr, ppConvertor);
//}
//bool ResourceLoader::GetImage(int resourceID, ID2D1Bitmap **formatConvertorPtrPtr, IWICFormatConverter **ppConvertor)
//{
//	return m_ImageLoaderPtr->LoadImage(resourceID, formatConvertorPtrPtr, ppConvertor);
//}