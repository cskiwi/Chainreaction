//-----------------------------------------------------------------
// Image Object
// C++ Source - Image.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#include "StdAfx.h"

#include "Image.h"
#include "ResourceLoader.h"
//---------------------------
// Image methods
//---------------------------
Image::Image(const String& fileName, ID2D1Bitmap *bitmapPtr, IWICFormatConverter *convertorPtr)
	:m_BitmapPtr(bitmapPtr)
	,m_ConvertorPtr(convertorPtr)
	,m_Opacity(1)
	,m_ResourceID(0)
	,m_FileName(fileName)
{

}
Image::Image(int resourceID, ID2D1Bitmap *bitmapPtr, IWICFormatConverter *convertorPtr)
	:m_BitmapPtr(bitmapPtr)
	,m_ConvertorPtr(convertorPtr)
	,m_Opacity(1)
	,m_ResourceID(resourceID)
	,m_FileName("")
{

}

//Image::Image(ResourceLoader* resourceLoaderPtr, const String& fileName)
//	:m_BitmapPtr(0)
//	,m_ConvertorPtr(0)
//	,m_Opacity(1)
//	,m_ResourceID(0)
//	,m_FileName("")
//{
//	bool bSuccess = resourceLoaderPtr->GetImage(fileName, &m_BitmapPtr, &m_ConvertorPtr);
//	if(!bSuccess)
//	{
//		//make sure window is validated
//		wcout << fileName.ToTChar() << L" NOT LOADED ERROR" << "\n";
//		MessageBoxW(NULL, fileName.ToTChar(), L"IMAGE LOADING ERROR",MB_ICONERROR);
//		exit(-1);
//	}
//	//bool bSuccess = resourceLoaderPtr->GetImage(fileName,);
//	//ImageLoader imageLoader2D(resourceLoaderPtr->GetT
//}
//Image::Image(ResourceLoader* resourceLoaderPtr, int resourceID)
//	:m_BitmapPtr(0)
//	,m_ConvertorPtr(0)
//	,m_Opacity(1)
//{
//	bool bSuccess = resourceLoaderPtr->GetImage(resourceID, &m_BitmapPtr, &m_ConvertorPtr);
//
//	//bool bSuccess = resourceLoaderPtr->GetImage(fileName,);
//	//ImageLoader imageLoader2D(resourceLoaderPtr->GetT
//}

Image::~Image()
{
	//SafeRelease(&m_ConvertorPtr);
	SafeRelease(&m_BitmapPtr);
}
