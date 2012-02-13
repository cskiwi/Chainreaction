//-----------------------------------------------------------------
// Button Object
// C++ Source - Button.h - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/

#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Caller.h"

class ResourceLoader;

//-----------------------------------------------------------------
// Button Class
//-----------------------------------------------------------------

class Button : public Caller
{
public:
	Button(HWND hWnd, String const& textRef);

	virtual ~Button();

	int GetType() {return Caller::Button;}
	void SetBounds(int x, int y, int width, int height);
	String GetText();
	void SetText(String const& textRef);
	void SetFont(String const& fontNameRef, bool bold, bool italic, bool underline, int size);
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
	HWND m_hWndButton;
	WNDPROC m_procOldButton;
	bool m_Armed;
	COLORREF m_BgColor, m_ForeColor;
	HFONT m_Font, m_OldFont;

	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK ButtonProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT ButtonProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Button(const Button& bRef);
	Button& operator=(const Button& bRef);
};