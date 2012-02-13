//-----------------------------------------------------------------
// MainEngine Object
// C++ Source - MainEngine.h - version 2011 v1_33
// Author Bart Uyttenhove 
//-----------------------------------------------------------------


#pragma once
#include <d2d1.h>
#include "Matrix2D.h"

#undef MessageBox

class AbstractGame;
class Painter;
class ResourceLoader;
class InputStateManager;
class InputState;
class GameTimer;
class String;
class GameConfigurator;

class MainEngine
{
public:
	MainEngine();
	virtual  ~MainEngine();
	
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HRESULT Initialize();

	void RunMessageLoop();
	
	//sets the pointer to the game
	void SetGame(AbstractGame *gamePtr){m_GamePtr = gamePtr;}

private:
	HRESULT Register_And_OpenWindow(const String &sTitle, int Width, int Height);

	HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();
	void OnResize(UINT width, UINT height);
	double GetCurrentGameTime();
	HRESULT Paint();
	void Tick(double deltaTime);

	void IsPaused(bool pause);
	bool IsPaused() const;

	void KeyUp(SHORT cKey);
	void OnMouseMove(LPARAM lParam);

	//Kevin Hoefman
	//void TabNext(HWND ChildWindow);
	//void TabPrevious(HWND ChildWindow);
	void MessageBox(double value);
	void MessageBox(int value);
	void MessageBox(size_t value);
	void MessageBox(String const& textRef);

	String m_WindowClassName;
	//Data members
	String m_sTitle;
	AbstractGame *m_GamePtr;
	Painter *m_PainterPtr;
	HWND m_Hwnd;
	bool m_bInitialized; //used to prevent app drawing before initializing is performed
	bool m_bTicked; //used to prevent paint before tick -->startup
	double	m_WindowScaleX;//used to scale mousepos
	double	m_WindowScaleY;//used to scale mousepos
	bool	m_bEnableWindowScale;
	double m_RenderTargetWidth, m_RenderTargetHeight;//the width of the rendertarget
	//key status array
	unsigned char m_Keys[256];
	ResourceLoader *m_ResourceLoaderPtr;
	InputStateManager *m_InputManagerPtr;
	ID2D1Factory *m_D2DFactoryPtr;
    IWICImagingFactory *m_WICFactoryPtr;
	ID2D1HwndRenderTarget *m_RenderTargetPtr;
	ID2D1SolidColorBrush *m_ColorBrushPtr;

	double m_PaintTime, m_TickTime;
	GameTimer *m_GameTimerPtr;
	GameConfigurator *m_GameConfiguratorPtr;

	bool m_WasClicked;
	Point2D m_MousePosAtClick;
	Matrix2D m_PainterTransformAtClick;
};