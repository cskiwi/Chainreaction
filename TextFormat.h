//-----------------------------------------------------------------
// TextFormat Object
// C++ Source - TextFormat.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#pragma once
class ResourceLoader;
class TextFormat
{
public:
	TextFormat(IDWriteTextFormat *textFormatPtr);
	virtual ~TextFormat(void);
	IDWriteTextFormat *GetTextFormat()const{return m_TextFormatPtr;}
	void SetAlignHLeft();
	void SetAlignHCenter();
	void SetAlignHRight();
	void SetAlignVTop();
	void SetAlignVCenter();
	void SetAlignVBottom();
private:
	IDWriteTextFormat *m_TextFormatPtr;

	TextFormat(const TextFormat& yRef);									
	TextFormat& operator=(const TextFormat& yRef);	

};

