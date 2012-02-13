//-----------------------------------------------------------------
// String Object
// C++ Source - String.h - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/

#pragma once
//-----------------------------------------------------------------
// String Class
//-----------------------------------------------------------------
class String
{
public: 
	// -------------------------
	// Constructors & Destructor
	// -------------------------	
	String(wchar_t const* wideTextPtr = L"");
	String(char const* singleTextPtr);
	String(String const& sRef);
	String(wchar_t character);
	String(char character);

	virtual ~String();

	// -------------------------
	// General String Methods
	// -------------------------	
	TCHAR CharAt(int index) const;
	String Replace(TCHAR oldChar, TCHAR newChar) const;
	String SubString(int index) const;
	String SubString(int index, int length) const;
	String ToLowerCase() const;
	String ToUpperCase() const;
	String Trim() const;
	int IndexOf(TCHAR character) const;
	int LastIndexOf(TCHAR character) const;
	bool StartsWith(String const& sRef) const;
	bool EndsWith(String const& sRef) const;
	int GetLength() const;
	bool Equals(String const& sRef) const;

	// -------------------------
	// Conversion Methods
	// -------------------------	
	TCHAR* ToTChar() const;
	int ToInteger() const;
	double ToDouble() const;

	// ----------------------------------------
	// Overloaded operators: = , +=, +, and ==
	// ----------------------------------------
	String& operator=(String const& sRef);

	String& operator+=(String const& sRef);
	String& operator+=(wchar_t* wideTextPtr);
	String& operator+=(char* singleTextPtr);
	String& operator+=(int number);
	String& operator+=(size_t number);
	String& operator+=(double number);
	String& operator+=(wchar_t character);
	String& operator+=(char character);

	String operator+(String const& sRef);
	String operator+(wchar_t* wideTextPtr);
	String operator+(char* singleTextPtr);
	String operator+(int number);
	String operator+(size_t number);
	String operator+(double number);
	String operator+(wchar_t character);
	String operator+(char character);

	bool operator==(String const& sRef);
	bool operator== (String const& sRef) const;

private:
	// -------------------------
	// Datamembers
	// -------------------------
	TCHAR* m_TextPtr;
	int m_Length;
};
