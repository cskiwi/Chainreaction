//-----------------------------------------------------------------
// TextFormatLoader Object
// C++ Source - TextFormatLoader.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#pragma once
#include "D2D1.h"

class TextFormat;

class TextFormatLoader
{
public:
	TextFormatLoader(void);
	virtual ~TextFormatLoader(void);
	TextFormat* LoadTextFormat(const wchar_t*fontName, float size);

private:
	HRESULT CreateWriteFactory();
	IDWriteFactory *m_DWriteFactoryPtr;

	TextFormatLoader(const TextFormatLoader& yRef);									
	TextFormatLoader& operator=(const TextFormatLoader& yRef);	
};

