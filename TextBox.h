//-----------------------------------------------------------------
// TextBox Object
// C++ Source - TextBox.h - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/


#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Caller.h"

class ResourceLoader;

//-----------------------------------------------------------------
// TextBox Class
//-----------------------------------------------------------------

class TextBox : public Caller
{
public:
	TextBox(HWND hWnd, String const& textRef);


	virtual ~TextBox();

	int GetType() {return Caller::TextBox;}
	void SetBounds(int x, int y, int width, int height);
	String GetText();
	void SetText(String const& textRef);
	void SetFont(String const& fontNameRef, bool bold, bool italic, bool underline, int size);
	void SetBackcolor( COLORREF color );
	void SetForecolor( COLORREF color );
	COLORREF GetForecolor();
	COLORREF GetBackcolor();
	HBRUSH GetBackcolorBrush();
	RECT GetRect();
	void SetEnabled(bool bEnable);
	void Update(void);
	void Show();
	void Hide();

private:
	// -------------------------
	// Datamembers
	// -------------------------
	int m_x, m_y;
	HWND m_hWndEdit;
	WNDPROC m_procOldEdit;
	COLORREF m_BgColor, m_ForeColor;
	HBRUSH m_BgColorBrush;
	HFONT m_Font, m_OldFont;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK EditProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT EditProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	TextBox(const TextBox& tbRef);
	TextBox& operator=(const TextBox& tbRef);
};