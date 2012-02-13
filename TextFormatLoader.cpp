//-----------------------------------------------------------------
// TextFormatLoader Object
// C++ Source - TextFormatLoader.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#include "StdAfx.h"
#include "TextFormatLoader.h"
#include <d2d1.h>
#include "TextFormat.h"

TextFormatLoader::TextFormatLoader(void)
	:m_DWriteFactoryPtr(0)
{
	CreateWriteFactory();
}


TextFormatLoader::~TextFormatLoader(void)
{
	SafeRelease(&m_DWriteFactoryPtr);
}

TextFormat* TextFormatLoader::LoadTextFormat(const wchar_t*fontName, float size)
{
	IDWriteTextFormat *textFormatPtr;
	HRESULT hr;
	// Create a DirectWrite text format object.
    hr = m_DWriteFactoryPtr->CreateTextFormat(
					fontName,
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					size,
					L"", //locale
					&textFormatPtr
					);

	if (SUCCEEDED(hr))
    {
        // align left and top the text horizontally and vertically.
        textFormatPtr->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        textFormatPtr->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		return new TextFormat(textFormatPtr);
    }

	MessageBoxW(NULL, fontName, L"TEXTFORMAT BUILDING ERROR",MB_ICONERROR);
	exit(-1);

	return 0;
}
HRESULT TextFormatLoader::CreateWriteFactory()
{
	HRESULT hr;
    // Create a DirectWrite factory.
    hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_DWriteFactoryPtr),
            reinterpret_cast<IUnknown **>(&m_DWriteFactoryPtr)
            );
	return hr;
}