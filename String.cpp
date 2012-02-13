//-----------------------------------------------------------------
// String Object
// C++ Source - String.cpp - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/

//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "String.h"

//-----------------------------------------------------------------
// String methods
//-----------------------------------------------------------------

String::String(wchar_t const* wideTextPtr)
{		
	m_Length = (int) wcslen(wideTextPtr) + 1; // include room for null terminator
	m_TextPtr = new TCHAR[m_Length]; 

	if (sizeof(TCHAR) == 2) _tcscpy_s(m_TextPtr, m_Length, (TCHAR*) wideTextPtr);
	else WideCharToMultiByte(CP_ACP, 0, wideTextPtr, -1, (char*) m_TextPtr, m_Length, NULL, NULL);
}

String::String(char const* singleTextPtr)
{
	m_Length = (int) strlen(singleTextPtr) + 1; // inlude room for null terminator

	m_TextPtr = new TCHAR[m_Length]; 
	
	if (sizeof(TCHAR) == 1) strcpy_s((char*) m_TextPtr, m_Length, singleTextPtr);
	else MultiByteToWideChar(CP_ACP, 0, singleTextPtr, -1, (wchar_t*) m_TextPtr, m_Length * 2);
}

String::String(String const& sRef)
{   
	m_Length = sRef.GetLength() + 1; // include room for null terminator
	m_TextPtr = new TCHAR[m_Length];

	_tcscpy_s(m_TextPtr, m_Length, sRef.ToTChar());
}

String::String(wchar_t character)
{
	m_Length = 2; // include room for null terminator
	m_TextPtr = new TCHAR[m_Length];

	m_TextPtr[0] = character;
	m_TextPtr[1] = '\0';
}

String::String(char character)
{
	m_Length = 2; // include room for null terminator
	m_TextPtr = new TCHAR[m_Length];

	m_TextPtr[0] = character;
	m_TextPtr[1] = '\0';
}

String::~String()
{
	delete m_TextPtr;
	m_TextPtr = 0;
}

String& String::operator=(String const& sRef)
{
	if (this != &sRef) // beware of self assignment: s = s
	{
		delete m_TextPtr;
		m_Length = sRef.GetLength() + 1;
		m_TextPtr = new TCHAR[m_Length];
		_tcscpy_s(m_TextPtr, m_Length, sRef.ToTChar());
	}
	return *this;
}
	
String& String::operator+=(String const& sRef)
{
	m_Length = this->GetLength() + sRef.GetLength() + 1;
	
	TCHAR* buffer = new TCHAR[m_Length];

	_tcscpy_s(buffer, m_Length, m_TextPtr);
	_tcscat_s(buffer, m_Length, sRef.m_TextPtr);

	delete m_TextPtr;
	m_TextPtr = buffer;

	return *this;
}

String& String::operator+=(wchar_t* wideTextPtr)
{
	return *this += String(wideTextPtr);
}

String& String::operator+=(char* singleTextPtr)
{
	return *this += String(singleTextPtr);
}

String& String::operator+=(int number)
{	
	char buffer[65]; // an int will never take more than 65 characters (int64 is max 20 characters)

	_itoa_s(number, buffer, sizeof(buffer), 10);

	return *this += String(buffer);
}

String& String::operator+=(size_t number)
{	
	char buffer[65]; // an int will never take more than 65 characters (int64 is max 20 characters)

	_ultoa_s((unsigned long) number, buffer, sizeof(buffer), 10);
	
	return *this += String(buffer);
}

String& String::operator+=(double number)
{
	//char buffer[_CVTBUFSIZE];

	//_gcvt_s(buffer, _CVTBUFSIZE, number, 40); // max 40 digits

	//if (number == (int) number) // _gcvt_s forgets a trailing 0 when there are no significant digits behind the comma, so add it manually
	//{
	//	size_t length = strlen(buffer);
	//	buffer[length] = '0';
	//	buffer[length+1] = '\0';
	//}
	//return *this += String(buffer);
	
	/*Bart Uyttenhove 19 okt 2009: geen conversie fouten door _gcvt_s  */
 	wstringstream strstr;
	strstr << number;
	wstring wstr;
	strstr >> wstr;
	return *this += String(wstr.c_str());


}

String& String::operator+=(wchar_t character)
{		
	return *this += String(character);
}

String& String::operator+=(char character)
{		
	return *this += String(character);
}

String String::operator+(String const& sRef)
{
	String newString = *this;

	newString += sRef;

	return newString;
}

String String::operator+(wchar_t* wideTextPtr)
{
	String newString = *this;

	newString += wideTextPtr;

	return newString;
}

String String::operator+(char* singleTextPtr)
{
	String newString = *this;

	newString += singleTextPtr;

	return newString;
}

String String::operator+(int number)
{
	String newString = *this;

	newString += number;

	return newString;
}

String String::operator+(size_t number)
{
	String newString = *this;

	newString += number;

	return newString;
}

String String::operator+(double number)
{ 
	String newString = *this;

	newString += number;

	return newString;
}

String String::operator+(wchar_t character)
{ 
	String newString = *this;

	newString += character;

	return newString;
}

String String::operator+(char character)
{ 
	String newString = *this;

	newString += character;

	return newString;
}

bool String::operator==(String const& sRef)
{
	return this->Equals(sRef);
}

bool String::operator==(String const& sRef) const
{
	return this->Equals(sRef);
}

TCHAR String::CharAt(int index) const
{
	if (index > this->GetLength() - 1) return 0;
	
	return m_TextPtr[index];
}

String String::Replace(TCHAR oldChar, TCHAR newChar) const
{
	String newString = *this;

	for (int count = 0; count < newString.GetLength(); count++)
	{
		if (newString.m_TextPtr[count] == oldChar) newString.m_TextPtr[count] = newChar;
	}
	
	return newString;
}

String String::SubString(int index) const
{
	if (index > this->GetLength() - 1) return String(""); 

	return String(m_TextPtr + index);
}

String String::SubString(int index, int length) const
{
	if (index + length - 1 > this->GetLength() - 1) return String(""); 

	String newString = *this;
	newString.m_TextPtr[index + length] = TEXT('\0');

	return String(newString.m_TextPtr + index);
}

String String::ToLowerCase() const
{
	String newString = *this;	

	for (int count = 0; count < newString.GetLength(); count++)
	{
		TCHAR character = newString.m_TextPtr[count];

		if (character < 91 && character > 64) newString.m_TextPtr[count] += 32;
	}

	return newString;
}

String String::ToUpperCase() const
{
	String newString = *this;	

	for (int count = 0; count < newString.GetLength(); count++)
	{
		TCHAR character = newString.m_TextPtr[count];

		if (character < 123 && character > 96) newString.m_TextPtr[count] -= 32;
	}

	return newString;
}


String String::Trim() const
{
	int beginIndex = 0;
	int endIndex = this->GetLength() - 1;

	while (m_TextPtr[beginIndex] == TEXT(' ') && m_TextPtr[beginIndex] != TEXT('\0')) ++beginIndex;
	while (m_TextPtr[endIndex] == TEXT(' ') && endIndex > 0) --endIndex;

	return this->SubString(beginIndex, endIndex - beginIndex + 1);
}

int String::IndexOf(TCHAR character) const
{
	int index = 0;

	while (m_TextPtr[index] != character && m_TextPtr[index] != TEXT('\0')) ++index;

	if (m_TextPtr[index] == character) return index;
	else return -1;
}

int String::LastIndexOf(TCHAR character) const
{
	int index = this->GetLength() - 1;

	while (m_TextPtr[index] != character && index > 0) --index;

	if (m_TextPtr[index] == character) return index;
	else return -1;
}

bool String::StartsWith(String const& sRef) const
{
	// return false if 2nd string is longer than 1st string 
	if (this->GetLength() < sRef.GetLength()) return false;

	// check individual characters
	bool result = true;
	int index = 0;
	int max = sRef.GetLength();

	while (index < max && result)
	{
		if (m_TextPtr[index] != sRef.m_TextPtr[index]) result = false;
		else ++index;
	}

	return result;
}

bool String::EndsWith(String const& sRef) const
{
	// return false if 2nd string is longer than 1st string 
	if (this->GetLength() < sRef.GetLength()) return false;

	String temp = this->SubString(this->GetLength() - sRef.GetLength());
	
	return sRef.Equals(temp);
}

int String::GetLength() const
{
	return m_Length - 1; // don't include the null terminator when asked how many characters are in the string
}

bool String::Equals(String const& sRef) const
{
	if (sRef.GetLength() != this->GetLength()) return false;

	return _tcscmp(this->ToTChar(), sRef.ToTChar())?false:true; // return true if cmp returns 0, false if not
}

int String::ToInteger() const
{
	return _tstoi(this->ToTChar());
}

double String::ToDouble() const
{
	return _tcstod(this->ToTChar(), 0);
}

TCHAR* String::ToTChar() const
{
	return m_TextPtr;
}