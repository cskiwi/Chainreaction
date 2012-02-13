//-----------------------------------------------------------------
// MainEngine Object
// C++ Source - MainEngine.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#include "StdAfx.h"

#include "MainEngine.h"
#include "AbstractGame.h"
#include "Painter.h"
#include "ResourceLoader.h"
#include "InputStateManager.h"
#include "GameConfigurator.h"

MainEngine::MainEngine() :
m_GamePtr(0),
	m_PainterPtr(0),
	m_Hwnd(NULL),
	m_D2DFactoryPtr(NULL),
	m_WICFactoryPtr(0),
	m_RenderTargetPtr(NULL),
	m_ColorBrushPtr(NULL),
	m_ResourceLoaderPtr(0),
	m_InputManagerPtr(0),
	m_bInitialized(false),
	m_bTicked(false)
	,m_PaintTime(0)
	,m_TickTime(0)
	,m_GameTimerPtr(0)
	,m_GameConfiguratorPtr(0)
	,m_WindowClassName("AMPEngine")
	,m_WindowScaleX(1.0)
	,m_WindowScaleY(1.0)
	,m_bEnableWindowScale(false)
	,m_RenderTargetWidth(0), m_RenderTargetHeight(0)
	,m_WasClicked(false)
{

}

MainEngine::~MainEngine()
{
	m_bTicked = false;
	m_bInitialized = false;
	SafeDelete(&m_GameConfiguratorPtr);
	SafeDelete(&m_PainterPtr);
	SafeDelete(&m_ResourceLoaderPtr);
	SafeDelete(&m_InputManagerPtr);
	SafeDelete(&m_GameTimerPtr);
	SafeRelease(&m_RenderTargetPtr);
	SafeRelease(&m_ColorBrushPtr);
	SafeRelease(&m_D2DFactoryPtr);
}

HRESULT MainEngine::Initialize()
{
	m_GameConfiguratorPtr = new GameConfigurator();
	m_GamePtr->GameInitialize(m_GameConfiguratorPtr);

	m_bEnableWindowScale = m_GameConfiguratorPtr->IsWindowScaleEnabled();
	if(m_bEnableWindowScale)
	{
		m_WindowScaleX = m_GameConfiguratorPtr->GetWindowScale();
		m_WindowScaleY = m_GameConfiguratorPtr->GetWindowScale();
	}
	m_RenderTargetWidth = m_GameConfiguratorPtr->GetRenderTargetWidth();
	m_RenderTargetHeight = m_GameConfiguratorPtr->GetRenderTargetHeight();

	//calculate windowsize from client area size
	RECT rSize;
	rSize.left = rSize.top = 0;
	if(m_bEnableWindowScale)
	{
		rSize.right = (LONG)(m_GameConfiguratorPtr->GetRenderTargetWidth()*m_WindowScaleX);
		rSize.bottom = (LONG)(m_GameConfiguratorPtr->GetRenderTargetHeight()*m_WindowScaleY);
	}
	else
	{
		rSize.right = m_GameConfiguratorPtr->GetRenderTargetWidth();
		rSize.bottom = m_GameConfiguratorPtr->GetRenderTargetHeight();
	}
	AdjustWindowRect(&rSize,WS_OVERLAPPEDWINDOW,FALSE);
	HRESULT hr = Register_And_OpenWindow(m_sTitle, rSize.right - rSize.left, rSize.bottom - rSize.top);

	return hr;
}

HRESULT MainEngine::Register_And_OpenWindow(const String &sTitle, int windowWidth, int windowHeight)
{
	HRESULT hr;
	hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr))
	{
		// Variabelen declareren die we gaan gebruiken


		WNDCLASS wndclass ;
		wndclass.style = CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc = MainEngine::WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = sizeof(LONG_PTR);
		wndclass.hInstance = GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = LoadCursor(NULL,IDC_ARROW) ;
		wndclass.hbrBackground = NULL;//(HBRUSH) CreateSolidBrush(RGB(188, 246, 115)) ; //use brush to enable auto clearing the backgroundd
		wndclass.lpszMenuName = NULL ;
		wndclass.lpszClassName = m_WindowClassName.ToTChar();

		if (!RegisterClass (&wndclass))
		{
			MessageBoxW (NULL, _T("RegisterClass failed"), m_WindowClassName.ToTChar(), MB_ICONERROR) ;
		}

		ULONG flags=0;
		if(m_GameConfiguratorPtr->IsWindowResizable())flags = WS_OVERLAPPEDWINDOW;
		else flags=WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
		//IMPORTANT: WS_CLIPCHILDREN IS NEEDED WHEN USING CHILD WINDOWS SUCH AS BUTTON AND TEXTBOX:
		//Excludes the area occupied by child windows when you draw within the parent window. Used when you create the parent window.
		m_Hwnd = CreateWindow(m_WindowClassName.ToTChar(),
			m_GameConfiguratorPtr->GetWindowTitle().ToTChar(),
			flags | WS_CLIPCHILDREN,
			(GetSystemMetrics(SM_CXSCREEN)-windowWidth)/2,
			(GetSystemMetrics(SM_CYSCREEN)-windowHeight)/2,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			GetModuleHandle(0),
			this);

		ShowWindow(m_Hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_Hwnd);
	}
	return hr;
}

void MainEngine::RunMessageLoop()
{
	// Seed the random number generator
	srand(GetTickCount());

	MSG msg= {0};	
	while(msg.message != WM_QUIT)
	{
		//Direct2D sends WMPAINT, synched with Vertical blanking interval
		//if(GetMessage (&msg, NULL, 0, 0))
		//{
		//	TranslateMessage (&msg);
		//	DispatchMessage (&msg);
		//}
		//below is not needed when paint message is used to paint
		//framerate is fixed at display vertical frequency
		//can be used to multiply physics calcs freq
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
		else
		{
			//initialize if not yet done
			if(m_bInitialized==false)
			{
				//INITIALIZE
				HRESULT hr=S_OK;

				hr = CreateDeviceResources();
				if(SUCCEEDED(hr))
				{
					//create ResourceLoader
					if(!m_ResourceLoaderPtr)m_ResourceLoaderPtr = new ResourceLoader(m_Hwnd, m_D2DFactoryPtr, m_RenderTargetPtr);

					//initialize game, load game content
					m_GamePtr->GameStart(m_ResourceLoaderPtr);
					m_bInitialized = true;
				}
			}
            if( m_GameTimerPtr->IsStopped() )
            {
                // Game is stopped.
                // Just repaint
                // Game resumes when PAUSE key is hit.
                Paint();
				Tick(0);
                Sleep(1);
            }
			else
			{
				double now = GetCurrentGameTime();

				int paintFreq = m_GameConfiguratorPtr->GetNumPaintsPerSecs();
				int tickFreq = m_GameConfiguratorPtr->GetNumTicksPerSecs();

				bool waitForSync = paintFreq == 0;
				bool tickOnPaint = tickFreq == 0;

				bool shouldPaint = waitForSync || now > m_PaintTime;

				if( shouldPaint )
				{
					// Paint just once, even when many paint periods passed,
					// since painting twice is redundant.
					Paint();

					if( !waitForSync )
					{
						double paintPeriod = 1.0 / paintFreq;

						do
						{
							m_PaintTime += paintPeriod;
						}
						while( now >= m_PaintTime );
					}
				}
				now = GetCurrentGameTime();

				if( tickOnPaint )
				{
					if( shouldPaint )
					{
						// measure timespan between last tick.
						now = GetCurrentGameTime();
						double deltaTime = now - m_TickTime;

						Tick(deltaTime);
						m_TickTime = now;
					}
				}
				else if( now >= m_TickTime )
				{
					// time to tick at fixed frequency.
					double tickPeriod = 1.0/tickFreq;

					while( now >= m_TickTime ) 
					{
						m_TickTime += tickPeriod;
						Tick(tickPeriod);
					}

					// Need slowdown? (happens when tick is slower than real-time)
					now = GetCurrentGameTime();

					if( m_TickTime < now )
					{
						m_TickTime = now;
					}
				}

				if( !waitForSync )
				{
					Sleep(1);
				}
			}
		}
	}
	m_GamePtr->GameEnd();
	UnregisterClass(m_WindowClassName.ToTChar(),0);
}

void MainEngine::IsPaused(bool pause)
{
	if( IsPaused() != pause )
	{
		if( pause )
		{
			SetWindowTextA(m_Hwnd, "*** GAME SUSPENDED *** press pause key to resume");
			m_GameTimerPtr->Stop();
		}
		else
		{
			SetWindowText(m_Hwnd, m_GameConfiguratorPtr->GetWindowTitle().ToTChar());
			m_GameTimerPtr->Start();

			// HACK: Code below is to get the first tick deltatime more or less right.
			// Needs deeper thought...
			int tickFreq = m_GameConfiguratorPtr->GetNumTicksPerSecs();
			double tickPeriod = tickFreq==0 ? 1.0/60 : 1.0/tickFreq;
			m_PaintTime = GetCurrentGameTime();
			m_TickTime = m_PaintTime-tickPeriod;
		}
	}
}

bool MainEngine::IsPaused() const
{
	return m_GameTimerPtr->IsStopped();
}

void MainEngine::Tick(double deltaTime)
{
	if( !m_bInitialized)  
		return;

	if( m_InputManagerPtr == nullptr )
		return;

	InputState *inputStatePtr = m_InputManagerPtr->GenerateInputState(m_PainterPtr->GetTransformMatrix(), deltaTime);

    if( m_GameTimerPtr == nullptr )
		return;

	if( m_GameTimerPtr->IsStopped() )
		return;

	if( deltaTime > m_GameConfiguratorPtr->GetMaxDeltaTime() )
		deltaTime = m_GameConfiguratorPtr->GetMaxDeltaTime();

    while( deltaTime > 0 )
    {
        inputStatePtr->SetDeltaTime(deltaTime);

        m_GamePtr->GameTick(inputStatePtr);

        if( inputStatePtr->IsPaused() )
        {
			IsPaused(true);
            break;
        }

        if( inputStatePtr->IsDirty() )
        {
            m_PaintTime = GetCurrentGameTime();
        }

        if( inputStatePtr->GetDeltaTime() <= 0 )
            break;

        deltaTime -= inputStatePtr->GetDeltaTime();
    }
}

double MainEngine::GetCurrentGameTime()
{
	m_GameTimerPtr->Tick();
	return m_GameTimerPtr->GetGameTime();
}

LRESULT CALLBACK MainEngine::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	// Eigen geschreven code voor de verschillende windows messages wordt hier geschreven
	// The Create Message is sent, even before the window is displayed
	if (message == WM_CREATE)
	{
		//Get the user data from lParam, containing this object's pointer
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		MainEngine *pApp = (MainEngine *)pcs->lpCreateParams;
		//Set the userdata in the window to this object's pointer
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pApp));

		result = 1;
	}
	else
	{
		//Get the pointer of this object, a static member funtion can not handle non static data or methods directly
		MainEngine *pApp = reinterpret_cast<MainEngine *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		if(pApp)
		{
			int zPos;
			switch (message)
			{
			case WM_PAINT:
			case WM_DISPLAYCHANGE:
				//ignore
				//ValidateRect(hwnd, NULL);
				if(pApp->m_bInitialized)
				{
					PAINTSTRUCT ps;
					BeginPaint(hwnd, &ps);
					pApp->Paint();
					EndPaint(hwnd, &ps);
				}
				else ValidateRect(hwnd, NULL);
				//	//
				//	//InvalidateRect(hwnd, NULL, FALSE);
				return 0;

			case WM_CTLCOLOREDIT:
				return SendMessage((HWND) lParam, WM_CTLCOLOREDIT, wParam, lParam);	// delegate this message to the child window

			case WM_CTLCOLORBTN:
				return SendMessage((HWND) lParam, WM_CTLCOLOREDIT, wParam, lParam);	// delegate this message to the child window

			case WM_MOUSEMOVE:
				pApp->OnMouseMove(lParam);
				return 0;

			case WM_LBUTTONDOWN:
				pApp->m_InputManagerPtr->SetLeftMouseButtonState(true);
				return 0;

			case WM_LBUTTONUP:
				pApp->m_InputManagerPtr->SetLeftMouseButtonState(false);
				return 0;

			case WM_RBUTTONDOWN:
				pApp->m_InputManagerPtr->SetRightMouseButtonState(true);
				return 0;

			case WM_RBUTTONUP:
				pApp->m_InputManagerPtr->SetRightMouseButtonState(false);
				return 0;

			case WM_MOUSEWHEEL:
				zPos = GET_WHEEL_DELTA_WPARAM(wParam)/120;
				pApp->m_InputManagerPtr->SetWheelPos(zPos);
				return 0;
			case WM_KEYUP:
				pApp->KeyUp(static_cast<SHORT>(wParam));
				pApp->m_GamePtr->KeyUp(static_cast<SHORT>(wParam));
				return 0;

			case WM_SYSCOMMAND:	// trapping this message prevents a freeze after the ALT key is released
				if (wParam == SC_KEYMENU) return 0;			// see win32 API : WM_KEYDOWN
				else break;   

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
				//case WM_ERASEBKGND://prevents screen flsh when resizing
				//	return 0;

			case WM_SIZE://execute code to resize rendertarget
				pApp->OnResize(LOWORD(lParam), HIWORD(lParam));
				if(wParam==SIZE_MINIMIZED)
				{
					pApp->IsPaused(true);
				}
				if(wParam==SIZE_RESTORED)
				{
					pApp->IsPaused(false);
				}
				return 0;
			}
		}
		return DefWindowProc (hwnd, message, wParam, lParam) ;
	}
	return result;
}


HRESULT MainEngine::Paint()
{
	HRESULT hr=S_OK;
	//
	//   hr = CreateDeviceResources();
	//if(SUCCEEDED(hr))
	//{
	//	//create ResourceLoader
	//	if(!m_ResourceLoaderPtr)m_ResourceLoaderPtr = new ResourceLoader(m_Hwnd, m_D2DFactoryPtr, m_RenderTargetPtr);

	//	//initialize game, load game content
	//	if(!m_bInitialized)
	//	{
	//		m_GamePtr->GameStart(m_ResourceLoaderPtr);
	//		m_bInitialized = true;
	//	}

	//}

	if (m_bInitialized==true /*&& SUCCEEDED(hr)*/ && !(m_RenderTargetPtr->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// Retrieve the size of the render target.
		D2D1_SIZE_F renderTargetSize = m_RenderTargetPtr->GetSize();

		m_RenderTargetPtr->BeginDraw();

		m_RenderTargetPtr->SetTransform(D2D1::Matrix3x2F::Identity());

		m_RenderTargetPtr->Clear(D2D1::ColorF(D2D1::ColorF::White));


		//met de cout is de animatie schokvrij, ook als er meer dan 1 cirkel wordt getekend

		m_GamePtr->GamePaint(m_PainterPtr);
		//zonder sleep een schokkerige animatie
		//blijkbaar moet er zekere tijd passeren in de Paint
		//Sleep(1);

		//hr = m_RenderTargetPtr->Flush();
		hr = m_RenderTargetPtr->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
			SafeDelete(&m_ResourceLoaderPtr);
			m_GamePtr->GameEnd();
			m_bInitialized = false;

		}
	}
	return hr;
}

//
// Create resources which are not bound
// to any device. Their lifetime effectively extends for the
// duration of the app. These resources include the Direct2D and
// DirectWrite factories,  and a DirectWrite Text Format object
// (used for identifying particular font characteristics).
//
HRESULT MainEngine::CreateDeviceIndependentResources()
{
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 50;
	HRESULT hr;
	ID2D1GeometrySink *pSink = NULL;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2DFactoryPtr);



	SafeRelease(&pSink);

	//create inut state manager
	m_InputManagerPtr = new InputStateManager();

	if(!m_GameTimerPtr)
	{
		m_GameTimerPtr = new GameTimer();
		m_GameTimerPtr->Reset();
	}

	return hr;
}

//
//  This method creates resources which are bound to a particular
//  Direct3D device. It's all centralized here, in case the resources
//  need to be recreated in case of Direct3D device loss (eg. display
//  change, remoting, removal of video card, etc).
//
HRESULT MainEngine::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_RenderTargetPtr)
	{

		/*        RECT rc;
		GetClientRect(m_Hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
		);
		*/
		D2D1_SIZE_U size = D2D1::SizeU(
			(UINT)m_RenderTargetWidth,
			(UINT)m_RenderTargetHeight
			);

		// Create a Direct2D render target.
		//use D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_NONE for vblank sync
		//and D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_IMMEDIATELY for no waiting
		//D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS   
		D2D1_PRESENT_OPTIONS pres_opt;
		if(m_GameConfiguratorPtr->GetWaitForDisplayRefresh())pres_opt = D2D1_PRESENT_OPTIONS_NONE;
		else pres_opt = D2D1_PRESENT_OPTIONS_IMMEDIATELY;

		//Peter: DPI setting van Display kan verschillen, waardoor client area niet correct afmetingen heeft
		auto rtp = D2D1::RenderTargetProperties();
		rtp.dpiX = 96;
		rtp.dpiY = 96;

		hr = m_D2DFactoryPtr->CreateHwndRenderTarget(
			rtp,
			D2D1::HwndRenderTargetProperties(m_Hwnd, size, pres_opt),
			&m_RenderTargetPtr
			);

		if (SUCCEEDED(hr))
		{
			m_PainterPtr = new Painter(m_RenderTargetPtr,m_D2DFactoryPtr);

			m_InputManagerPtr->SetWidth(m_PainterPtr->GetWidth());
			m_InputManagerPtr->SetHeight(m_PainterPtr->GetHeight());

		}
	}
	return hr;
}


//
//  Discard device-specific resources which need to be recreated
//  when a Direct3D device is lost
//
void MainEngine::DiscardDeviceResources()
{
	cout << L"Discart Device Resources\n";
	m_bInitialized = false;
	m_GamePtr->GameEnd();
	SafeRelease(&m_RenderTargetPtr);
	SafeRelease(&m_ColorBrushPtr);
	SafeDelete(&m_PainterPtr);
	SafeDelete(&m_ResourceLoaderPtr);
}
//
//  If the application receives a WM_SIZE message, this method
//  resizes the render target appropriately.
//
void MainEngine::OnResize(UINT width, UINT height)
{
	if (m_RenderTargetPtr)
	{
		if(!m_bEnableWindowScale)
		{
			m_RenderTargetWidth = width;
			m_RenderTargetHeight = height;

			D2D1_SIZE_U size;
			size.width = width;
			size.height = height;

			// Note: This method can fail, but it's okay to ignore the
			// error here -- it will be repeated on the next call to
			// EndDraw.
			m_RenderTargetPtr->Resize(size);
			m_InputManagerPtr->SetWidth(m_PainterPtr->GetWidth());
			m_InputManagerPtr->SetHeight(m_PainterPtr->GetHeight());

			m_GamePtr->WindowResized(width, height);
			m_WindowScaleX=1.0;
			m_WindowScaleY=1.0;
		}
		//window content is scaled by the OS
		else
		{
			m_WindowScaleX = width / m_RenderTargetWidth;
			m_WindowScaleY = height / m_RenderTargetHeight;
		}
	}
}
void MainEngine::KeyUp(SHORT cKey)
{
	switch(cKey)
	{
	case 19:
		//pause toets
		IsPaused( !IsPaused() );
		break;
	}
}

void MainEngine::OnMouseMove(LPARAM lParam)
{
	float dpiX = 96.0f;
	float dpiY = 96.0f;

	if (m_RenderTargetPtr)
	{
		m_RenderTargetPtr->GetDpi(&dpiX, &dpiY);
	}
	m_InputManagerPtr->SetMousePos((double)LOWORD(lParam) * 96.0f / dpiX / m_WindowScaleX, (double)HIWORD(lParam) * 96.0f / dpiY / m_WindowScaleY);
}

void MainEngine::MessageBox(String const& textRef)
{
	if (sizeof(TCHAR) == 2)	MessageBoxW(m_Hwnd, (wchar_t*) textRef.ToTChar(), (wchar_t*) m_sTitle.ToTChar(), MB_ICONEXCLAMATION | MB_OK);
	else MessageBoxA(m_Hwnd, (char*) textRef.ToTChar(), (char*) m_sTitle.ToTChar(), MB_ICONEXCLAMATION | MB_OK);
}

void MainEngine::MessageBox(int value)
{
	MessageBox(String("") + value);
}

void MainEngine::MessageBox(size_t value)
{
	MessageBox(String("") + value);
}

void MainEngine::MessageBox(double value) 
{
	MessageBox(String("") + value);
}


//to enable tab
//static bool CALLBACK EnumInsertChildrenProc(HWND hwnd, LPARAM lParam)
//{
//	vector<HWND> *row = (vector<HWND> *) lParam; 
//
//	row->push_back(hwnd); // elk element invullen in de vector
//
//	return true;
//}
//
//void MainEngine::TabNext(HWND ChildWindow)
//{
//	vector<HWND> childWindows; 
//
//	EnumChildWindows(m_Hwnd , (WNDENUMPROC) EnumInsertChildrenProc, (LPARAM) &childWindows);
//
//	int position = 0;
//	HWND temp = childWindows[position];
//	while(temp != ChildWindow) temp = childWindows[++position]; // positie van childWindow in de vector opzoeken 
//
//	if (position == childWindows.size() - 1) SetFocus(childWindows[0]);
//	else SetFocus(childWindows[position + 1]);
//}
//
//void MainEngine::TabPrevious(HWND ChildWindow)
//{	
//	vector<HWND> childWindows;
//
//	EnumChildWindows(m_Hwnd, (WNDENUMPROC) EnumInsertChildrenProc, (LPARAM) &childWindows);
//
//	int position = (int) childWindows.size() - 1;
//	HWND temp = childWindows[position];
//	while(temp != ChildWindow) temp = childWindows[--position]; // positie van childWindow in de vector opzoeken 
//
//	if (position == 0) SetFocus(childWindows[childWindows.size() - 1]);
//	else SetFocus(childWindows[position - 1]);
//}