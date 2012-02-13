//-----------------------------------------------------------------
// TextFormat Object
// C++ Source - TextFormat.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#include "StdAfx.h"
#include "TextFormat.h"
#include "TextFormatLoader.h"
#include <d2d1.h>
#include "ResourceLoader.h"


TextFormat::TextFormat(IDWriteTextFormat *textFormatPtr)
	:m_TextFormatPtr(textFormatPtr)
{

}


TextFormat::~TextFormat(void)
{
	SafeRelease(&m_TextFormatPtr);
}

void TextFormat::SetAlignHLeft()
{
	m_TextFormatPtr->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
}

void TextFormat::SetAlignHCenter()
{
	m_TextFormatPtr->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void TextFormat::SetAlignHRight()
{
	m_TextFormatPtr->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
}

void TextFormat::SetAlignVTop()
{
	m_TextFormatPtr->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void TextFormat::SetAlignVCenter()
{
	m_TextFormatPtr->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void TextFormat::SetAlignVBottom()
{
	m_TextFormatPtr->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
}
