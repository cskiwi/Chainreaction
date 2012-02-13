#include "StdAfx.h"

#include "ImageLoader.h"
#include "Image.h"

//---------------------------
// ImageLoader methods
//---------------------------

ImageLoader::ImageLoader(ID2D1RenderTarget *renderTargetPtr, IWICImagingFactory *wICFactoryPtr)
	:m_RenderTargetPtr(renderTargetPtr)
	,m_WICFactoryPtr(wICFactoryPtr)
{

}

ImageLoader::~ImageLoader()
{
}

Image* ImageLoader::LoadImage(int resourceID)
{
	ID2D1Bitmap *bitmapPtr=0;
	IWICFormatConverter *convertorPtr=0;

	HRESULT hr = LoadResourceBitmap(
		m_RenderTargetPtr,
		m_WICFactoryPtr,
		static_cast<unsigned int> (resourceID),
		L"Image",
		&convertorPtr
		);
	if (SUCCEEDED(hr))
	{
		//Create a Direct2D bitmap from the WIC bitmap.
		hr = m_RenderTargetPtr->CreateBitmapFromWicBitmap(
			convertorPtr,
			&bitmapPtr
			);
	}

	if(FAILED(hr))
	{
		SafeRelease(&convertorPtr);
		SafeRelease(&bitmapPtr);
		//show messagebox and leave the program
		wchar_t buffer[80];
		_itow_s(resourceID,buffer,size_t(80),10);
		MessageBoxW(NULL, buffer, L"RESOURCE IMAGE LOADING ERROR",MB_ICONERROR);
		exit(-1);
		return 0;
	}
	return new Image(resourceID, bitmapPtr, convertorPtr);
}
//bool ImageLoader::LoadImage(int resourceID, ID2D1Bitmap **formatConvertorPtrPtr, IWICFormatConverter **ppConvertor)
//{
//
//	HRESULT hr = LoadResourceBitmap(
//		m_RenderTargetPtr,
//		m_WICFactoryPtr,
//		static_cast<unsigned int> (resourceID),
//		L"Image",
//		ppConvertor
//		);
//	if (SUCCEEDED(hr))
//	{
//		 Create a Direct2D bitmap from the WIC bitmap.
//		hr = m_RenderTargetPtr->CreateBitmapFromWicBitmap(
//			*ppConvertor,
//			formatConvertorPtrPtr
//			);
//	}
//
//	if(FAILED(hr))
//	{
//		SafeRelease(ppConvertor);
//		SafeRelease(formatConvertorPtrPtr);
//		return false;
//	}
//	return true;
//}


Image* ImageLoader::LoadImage(const String& fileName)
{
	ID2D1Bitmap *bitmapPtr=0;
	IWICFormatConverter *convertorPtr=0;
	HRESULT hr = LoadBitmapFromFile(
		m_RenderTargetPtr,
		m_WICFactoryPtr,
		fileName.ToTChar(),
		0,0,
		&convertorPtr
		);
	if (SUCCEEDED(hr))
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = m_RenderTargetPtr->CreateBitmapFromWicBitmap(
			convertorPtr,
			&bitmapPtr
			);
	}

	if(FAILED(hr))
	{
		SafeRelease(&convertorPtr);
		SafeRelease(&bitmapPtr);
		//show messagebox and leave the program
		MessageBoxW(NULL, fileName.ToTChar(), L"IMAGE LOADING ERROR",MB_ICONERROR);
		exit(-1);
	}
	return new Image(fileName,bitmapPtr,convertorPtr);
}


/******************************************************************
*        Direct2D                                                         *
*  This method will create a Direct2D bitmap from an application  *
*  resource.                                                      *
*                                                                 *
******************************************************************/

HRESULT ImageLoader::LoadResourceBitmap(
	ID2D1RenderTarget *renderTargetPtr,
	IWICImagingFactory *wICFactoryPtr,
	unsigned int resourceNumber,
	PCWSTR resourceType,
	IWICFormatConverter **ppFormatConverter
	)
{
	wstringstream strstr;
	strstr << "#" << resourceNumber;
	return LoadResourceBitmap(renderTargetPtr, wICFactoryPtr, (strstr.str().c_str()), resourceType, ppFormatConverter);
}

HRESULT ImageLoader::LoadResourceBitmap(
	ID2D1RenderTarget *renderTargetPtr,
	IWICImagingFactory *wICFactoryPtr,
	PCWSTR resourceName,
	PCWSTR resourceType,
	IWICFormatConverter **ppFormatConverter
	)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void *pImageFile = NULL;
	DWORD imageFileSize = 0;
	//http://msdn.microsoft.com/en-us/library/ms648042(VS.85).aspx
	//als het geen pointer is, dan is het een resource nummer
	BOOL isIntResource = IS_INTRESOURCE(resourceName);
	// Locate the resource.
	imageResHandle = FindResourceW(GetModuleHandle(0), resourceName, resourceType);

	hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// Load the resource.
		imageResDataHandle = LoadResource(GetModuleHandle(0), imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// Lock it to get a system memory pointer.
		pImageFile = LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// Calculate the size.
		imageFileSize = SizeofResource(GetModuleHandle(0), imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// Create a WIC stream to map onto the memory.
		hr = wICFactoryPtr->CreateStream(&pStream);
	}

	if (SUCCEEDED(hr))
	{
		// Initialize the stream with the memory pointer and size.
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
			);
	}

	if (SUCCEEDED(hr))
	{
		// Create a decoder for the stream.
		hr = wICFactoryPtr->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
			);
	}

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = wICFactoryPtr->CreateFormatConverter(ppFormatConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = (*ppFormatConverter)->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
			);
	}



	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	return hr;
}
//
// Creates a Direct2D bitmap from the specified
// file name.
//
HRESULT ImageLoader::LoadBitmapFromFile(
	ID2D1RenderTarget *renderTargetPtr,
	IWICImagingFactory *wICFactoryPtr,
	PCWSTR uri,
	UINT destinationWidth,
	UINT destinationHeight,
	IWICFormatConverter **ppFormatConverter
	)
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr = wICFactoryPtr->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
		);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = wICFactoryPtr->CreateFormatConverter(ppFormatConverter);

	}


	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an
		// IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = wICFactoryPtr->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
						);
				}
				if (SUCCEEDED(hr))
				{
					hr = (*ppFormatConverter)->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
						);
				}
			}
		}
		else // Don't scale the image.
		{
			hr = (*ppFormatConverter)->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
				);
		}
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pScaler);

	return hr;
}
